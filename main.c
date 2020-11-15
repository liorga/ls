#include "my_ls.h"
// includes all the libs and defines for the program

int main(int argc , char* argv[]) {
	
    switch (argc) {
        case 1:{
            ls_start("."); //if no args gives print the current dir stats
            break;
        }
        case 2:{
            ls_start(argv[1]);
            break;
        }

        default:{ //if more then 2 args then handle all of them
            for (int i = 1; i < argc ; ++i) {
	            struct stat buff;
	            lstat(argv[i],&buff);
	            ls_start(argv[i]);
	            if(S_ISDIR(buff.st_mode)){
		            printf(GREEN);
		            printf("directory name: %s\n",dirname(argv[i]));
		            printf(DEFAULT_COLOR);
	            }
	            printf("\n");

            }
            break;
        }

    }

	return 0;
}


void ls_start(char* file_path){ // the main function in the program
    DIR* dp;
    char name[PATH_MAX];
    struct dirent* dir_p;
    struct stat buff;

    lstat(file_path,&buff); // gets file stats to check its mode later

    /**
     * check if file is dir link or reg
     * if link then printing its info
     * if reg then printing its info
     * if dir then printing all of the dir files info
     */
    if(S_ISREG(buff.st_mode)){
        printFileStats(&buff);
        printf("%s\n",file_path);
    }
    else if (S_ISLNK(buff.st_mode)){
        printSymbolicLinkStats(&buff,file_path);
        printf("\n");
    } else {
        chdir(file_path); //changing the working dor to be the given path dir
        dp = opendir("."); //open the current dir that has changed line before to make sure im on the right dir
        /**
         * reading all of the dir files content and prints the information according to the file mode(dir,lnk,reg)
         * and making sure to print them in different colors
         */
        while ((dir_p = readdir(dp)) != NULL) { //storing dir info inside dirent struct
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
	/**
	 * prints the file permissions
	 */
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

    //prints number of hard links the file has
    printf("%lu\t", fileStats->st_nlink);

    //print the file user and group names
    struct group *group;
    struct passwd *passwd;
    group = getgrgid(fileStats->st_gid);
    printf( "%s", group->gr_name);
    passwd = getpwuid(fileStats->st_uid);
    printf(" %s\t", passwd->pw_name);

	// file length in bytes
    printf(" %zu",fileStats->st_size);

	//printing the file last modify date
    char date[NAME_];
    strftime(date, NAME_, " %b %d %X\t", localtime(&(fileStats->st_mtime)));
    printf("\t%s",date);

}

void printSymbolicLinkStats(struct stat* symLnk,char* dir_p){
	/**
	 * prints the file permissions
	 */
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
	
	//prints number of hard links the file has
    printf("%lu\t", symLnk->st_nlink);
	
	//print the file user and group names
    struct group *group;
    struct passwd *passwd;
    group = getgrgid(symLnk->st_gid);
    printf( "%s", group->gr_name);
    passwd = getpwuid(symLnk->st_uid);
    printf(" %s\t", passwd->pw_name);
    
	// file length in bytes
    printf(" %zu",symLnk->st_size);
	
	//printing the file last modify date
    char date[NAME_];
    strftime(date, NAME_, " %b %d %X\t", localtime(&(symLnk->st_mtime)));
    printf("\t%s",date);
    
    // handle the symbolic link status and print not only the link but also the file that is linked to
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
