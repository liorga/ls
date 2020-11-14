#include "my_ls.h"

int main(int argc , char* argv[]) {

    switch (argc) {
        case 1:{
            ls_start(".");
            break;
        }
        case 2:{
            ls_start(argv[1]);
            break;
        }
        default:{
            printf("wrong num of arguments\n");
            exit(1);
        }
    }

	return 0;
}


void ls_start(char* file_path){
    DIR* dp;
    struct dirent* dir_p;
    struct stat buff;
    lstat(file_path,&buff);
    if(S_ISREG(buff.st_mode)){
        printFileStats(&buff);
        printf("\t%s\n",file_path);
    }
    else if (S_ISLNK(buff.st_mode)){
        printSymbolicLinkStats(&buff,file_path);
        printf("\n");
    } else {
        chdir(file_path);
        dp = opendir(".");
        char name[PATH_MAX];
        while ((dir_p = readdir(dp)) != NULL) {
            sprintf(name, "%s/%s", file_path, dir_p->d_name);
            lstat(dir_p->d_name, &buff);
            if (S_ISLNK(buff.st_mode)) {
                printSymbolicLinkStats(&buff, dir_p->d_name);
            } else {
                printFileStats(&buff);
                if (S_ISDIR(buff.st_mode)) {
                    printf(GREEN);
                }
                if (S_ISREG(buff.st_mode)) {
                    printf(RED);
                }
                printf("\t%s", dir_p->d_name);
                printf(DEFAULT_COLOR);
            }
            printf("\n");
        }

        closedir(dp);
    }
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

    printf("%lu\t", fileStats->st_nlink);

    struct group *grp;
    struct passwd *pwd;
    grp = getgrgid(fileStats->st_gid);
    printf( "\t%s", grp->gr_name);
    pwd = getpwuid(fileStats->st_uid);
    printf(" %s\t\t", pwd->pw_name);


    printf(" %zu",fileStats->st_size);


    char date[NAME_];

    strftime(date, NAME_, " %b %d %Y\t", localtime(&(fileStats->st_mtime)));
    printf("\t%s",date);

}

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

    printf("%lu\t", symLnk->st_nlink);


    struct group *grp;
    struct passwd *pwd;
    grp = getgrgid(symLnk->st_gid);
    printf( "\t%s", grp->gr_name);
    pwd = getpwuid(symLnk->st_uid);
    printf(" %s\t\t", pwd->pw_name);

    printf(" %zu",symLnk->st_size);


    char date[NAME_];
    strftime(date, NAME_, " %b %d %Y\t", localtime(&(symLnk->st_mtime)));

    printf("\t%s",date);
    char* buff;
    ssize_t bufsize ,nbytes;
    bufsize = symLnk->st_size + 1;
    if (symLnk->st_size == 0) {
        bufsize = PATH_MAX;
    }
    buff = malloc(bufsize);
    nbytes = readlink(dir_p,buff,bufsize);
    printf(CYAN);
    printf("\t%s -> %.*s", dir_p, (int) nbytes, buff);
    printf(DEFAULT_COLOR);
    free(buff);
}
