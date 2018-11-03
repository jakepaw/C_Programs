/*
Author: Jacob Pawlak
Program Purpose: Rewriting the myfind command to print if a str matches files / dirs.
Prints out the dir it is in, the file or dir, octal perms, and rwx perms
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void do_ls(char [], char []);
void print_perm(struct stat fileStat);

int main(int ac, char *av[]){

    if(ac == 1)
        printf("Substring must be provided for myfind\n\n");
    else if (ac == 2)
        do_ls(".", av[1]);
    else if (ac == 3)
        do_ls(av[2], av[1]);
    return 1;
}

void do_ls(char dirname[], char searchTerm[]){
    DIR *dir_ptr; // directory
    char actualpath [PATH_MAX +1];
    struct dirent *direntp; // entry
    if((dir_ptr = opendir(dirname)) == NULL);
        //fprintf(stderr, "ls: cannot open %s\n", dirname);
    else{
        // scan for files with substring
        int dirPrinted = 0;
        while((direntp = readdir(dir_ptr)) != NULL){
            if(strstr(direntp->d_name, searchTerm) != 0){
                if(dirPrinted == 0){
                    // real path to find the full path ?
                    printf("%s\n", realpath(dirname, actualpath));
                    dirPrinted = 1;
                }
                printf("\t%s ", direntp->d_name);
                struct stat fileStat;
                // create file directory path
                char newDir[strlen(dirname) + strlen(direntp->d_name) + 1];
                strcpy(newDir, dirname);
                strcat(newDir, "/");
                strcat(newDir, direntp->d_name);

                stat(realpath(newDir, actualpath),&fileStat);
                printf("(");
                print_perm(fileStat);
                printf(")\n");
            }
        }

        closedir(dir_ptr);
        dir_ptr = opendir(dirname);

        // scan for directories to search
        while((direntp = readdir(dir_ptr)) != NULL){
            if(direntp->d_type == 4 && direntp->d_name[0] != '.'){
                // Combine previous directory with new directory
                char newDir[strlen(dirname) + strlen(direntp->d_name) + 1];
                strcpy(newDir, dirname);
                strcat(newDir, "/");
                strcat(newDir, direntp->d_name);
                // do_ls with new directory
                do_ls(newDir, searchTerm);
            }
        }
        closedir(dir_ptr);
    }
}

void print_perm(struct stat fileStat){
    char str[64];
    strcpy( str, "----------" );           /* default=no perms */

    int fileStatchmod = fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);

    if ( S_ISDIR(fileStat.st_mode) )  str[0] = 'd';    /* directory?       */
    if ( S_ISCHR(fileStat.st_mode) )  str[0] = 'c';    /* char devices     */
    if ( S_ISBLK(fileStat.st_mode) )  str[0] = 'b';    /* block device     */

    if ( fileStat.st_mode & S_IRUSR ) str[1] = 'r';    /* 3 bits for user  */
    if ( fileStat.st_mode & S_IWUSR ) str[2] = 'w';
    if ( fileStat.st_mode & S_IXUSR ) str[3] = 'x';

    if ( fileStat.st_mode & S_IRGRP ) str[4] = 'r';    /* 3 bits for group */
    if ( fileStat.st_mode & S_IWGRP ) str[5] = 'w';
    if ( fileStat.st_mode & S_IXGRP ) str[6] = 'x';

    if ( fileStat.st_mode & S_IROTH ) str[7] = 'r';    /* 3 bits for other */
    if ( fileStat.st_mode & S_IWOTH ) str[8] = 'w';
    if ( fileStat.st_mode & S_IXOTH ) str[9] = 'x';

    printf("0%o/%s",fileStatchmod, str);
}
