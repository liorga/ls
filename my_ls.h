//
// Created by lior on 14/11/2020.
//

#ifndef LS_MY_LS_H
#define LS_MY_LS_H

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>
#include <libgen.h>

#define NAME_ 256

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define DEFAULT_COLOR "\033[0m"
#define CYAN "\033[0;36m"


void printSymbolicLinkStats(struct stat* symLnk,char* dir_p);
void printFileStats(struct stat* fileStats);
void ls_start(char* file_path);

#endif //LS_MY_LS_H
