#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>

void printSymbolicLinkStats(struct stat* symLnk,char* dir_p){
	printf( (S_ISLNK(symLnk->st_mode)) ? "l" : "-");
	printf( (symLnk->st_mode & S_IRUSR) ? "r" : "-");
	printf( (symLnk->st_mode & S_IWUSR) ? "w" : "-");
	printf( (symLnk->st_mode & S_IXUSR) ? "x" : "-");
	printf( (symLnk->st_mode & S_IRGRP) ? "r" : "-");
	printf( (symLnk->st_mode & S_IWGRP) ? "w" : "-");
	printf( (symLnk->st_mode & S_IXGRP) ? "x" : "-");
	printf( (symLnk->st_mode & S_IROTH) ? "r" : "-");
	printf( (symLnk->st_mode & S_IWOTH) ? "w" : "-");
	printf( (symLnk->st_mode & S_IXOTH) ? "x\t" : "-\t");
	
	printf("%d\t", symLnk->st_nlink);


	struct group *grp;
	struct passwd *pwd;
	grp = getgrgid(symLnk->st_gid);
	printf( "\t%s", grp->gr_name);
	pwd = getpwuid(symLnk->st_uid);
	printf(" %s\t\t", pwd->pw_name);

	printf(" %zu",symLnk->st_size);
    if(symLnk->st_size < 100){
        printf("\t");
    }


	char date[20];
	strftime(date, 30, " %b %d %Y\t", localtime(&(symLnk->st_mtime)));
	printf("\t%s",date);
	char* buff;
	ssize_t bufsize ,nbytes;
	bufsize = symLnk->st_size + 1;
	if (symLnk->st_size == 0) {
		bufsize = PATH_MAX;
	}
	buff = malloc(bufsize);
	nbytes = readlink(dir_p,buff,bufsize);
	printf("\033[0;34m");
	printf("\t%s -> %.*s", dir_p, (int) nbytes, buff);
	printf("\033[0m");
	free(buff);
}

void printFileStats(struct stat* fileStats){
	printf( (S_ISDIR(fileStats->st_mode)) ? "d" : "-");
	printf( (fileStats->st_mode & S_IRUSR) ? "r" : "-");
	printf( (fileStats->st_mode & S_IWUSR) ? "w" : "-");
	printf( (fileStats->st_mode & S_IXUSR) ? "x" : "-");
	printf( (fileStats->st_mode & S_IRGRP) ? "r" : "-");
	printf( (fileStats->st_mode & S_IWGRP) ? "w" : "-");
	printf( (fileStats->st_mode & S_IXGRP) ? "x" : "-");
	printf( (fileStats->st_mode & S_IROTH) ? "r" : "-");
	printf( (fileStats->st_mode & S_IWOTH) ? "w" : "-");
	printf( (fileStats->st_mode & S_IXOTH) ? "x\t" : "-\t");
	
	printf("%d\t", fileStats->st_nlink);

	struct group *grp;
	struct passwd *pwd;
	grp = getgrgid(fileStats->st_gid);
	printf( "\t%s", grp->gr_name);
	pwd = getpwuid(fileStats->st_uid);
	printf(" %s\t\t", pwd->pw_name);

	
	printf(" %zu",fileStats->st_size);
	if(fileStats->st_size < 100){
	    printf("\t");
	}
	
	char date[20];
	strftime(date, 30, " %b %d %Y\t", localtime(&(fileStats->st_mtime)));
	printf("\t%s",date);

}

int main(int argc , char* argv[]) {
	
	DIR* dp;
	struct dirent* dir_p;
	struct stat buff;
	chdir(argv[1]);
	dp = opendir("..");
	char name[PATH_MAX];
	while((dir_p = readdir(dp)) != NULL){
		sprintf(name, "%s/%s", argv[1], dir_p->d_name);
		lstat(dir_p->d_name,&buff);
		if(S_ISLNK(buff.st_mode)) {
			printSymbolicLinkStats(&buff,dir_p->d_name);
		}
		else {
			printFileStats(&buff);
			if(S_ISDIR(buff.st_mode)){
			    printf("\033[0;32m");
			}
			if(S_ISREG(buff.st_mode)){
                printf("\033[0;31m");
			}
			printf("\t%s", dir_p->d_name);
			printf("\033[0m");
		}
		printf("\n");
	}
	
	closedir(dp);
	return 0;
}