#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void printFileStats(struct stat fileStats){
	printf( (S_ISDIR(fileStats.st_mode)) ? "d" : "-");
	printf( (fileStats.st_mode & S_IRUSR) ? "r" : "-");
	printf( (fileStats.st_mode & S_IWUSR) ? "w" : "-");
	printf( (fileStats.st_mode & S_IXUSR) ? "x" : "-");
	printf( (fileStats.st_mode & S_IRGRP) ? "r" : "-");
	printf( (fileStats.st_mode & S_IWGRP) ? "w" : "-");
	printf( (fileStats.st_mode & S_IXGRP) ? "x" : "-");
	printf( (fileStats.st_mode & S_IROTH) ? "r" : "-");
	printf( (fileStats.st_mode & S_IWOTH) ? "w" : "-");
	printf( (fileStats.st_mode & S_IXOTH) ? "x" : "-");
	
	printf("\t%d", fileStats.st_nlink);
	
	struct group *grp;
	struct passwd *pwd;
	grp = getgrgid(fileStats.st_gid);
	printf( "\t%s", grp->gr_name);
	pwd = getpwuid(fileStats.st_uid);
	printf("\t%s", pwd->pw_name);
	
	
	printf("\t%zu",fileStats.st_size);
	
	char date[20];
	strftime(date, 30, "\t%b %Y %X", localtime(&(fileStats.st_mtime)));
	printf("\t%s",date);
	
	//printf("\t%d", fileStats.st_ino);
	//printf(" The file %s a symbolic link ", (S_ISLNK(fileStats.st_mode)) ? "is" : "is not");
	
}

int main(int argc , char* argv[]) {
	
	DIR* dp;
	struct dirent* dir_p;
	struct stat buff;
	dp = opendir(argv[1]);
	char name[512];
	while((dir_p = readdir(dp)) != NULL){
		sprintf(name, "%s/%s", argv[1], dir_p->d_name);
		stat(dir_p->d_name,&buff);
		printFileStats(buff);
		printf("\t%s\n", dir_p->d_name);
	}
	
	closedir(dp);
	//printf("%i %s %P\n",buff.st_ino,buff.st_size,buff.st_mode);
	return 0;
}