#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "minish.h"

void
swap(struct dirent **dir_entry_1, struct dirent **dir_entry_2) {
    struct dirent *temp = *dir_entry_1;
    *dir_entry_1 = *dir_entry_2;
    *dir_entry_2 = temp;
}

int 
builtin_dir(int argc, char **argv) {
    if (argc > 3) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"dir\"\n");
        char *help_argv[] = {"help", "dir"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE; 
    }

    DIR *directory;
    struct dirent *dir_entry_arr[1024];
    int dirent_arr_size = 0;
    if (argc == 1)
        directory = opendir(".");
    if (argc == 2 || argc == 3) 
        directory = opendir(argv[1]); //aca hay un error

    if (directory != NULL) {
        struct dirent *dir_entry;
        while ((dir_entry = readdir(directory)) != NULL) { // si arranca con punto lo ignoro como el -l?
            int i = dirent_arr_size - 1;
            dir_entry_arr[dirent_arr_size] = dir_entry;
            dirent_arr_size++;
            while (i >= 0 && strcmp(dir_entry->d_name, dir_entry_arr[i]->d_name) < 0) {
                swap(&dir_entry_arr[i + 1], &dir_entry_arr[i]);
                --i;
            }
        }
        struct stat fileStat;
        for (int i = 0; i < dirent_arr_size; i++) {

            if(stat(dir_entry_arr[i]->d_name, &fileStat) < 0) {
                printf("Error: Error al obtener la informacion de \"%s\"\n", dir_entry_arr[i]->d_name);
                //continue;
                //return EXIT_FAILURE;
            }   
    
            printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
            printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
            printf(" %ld", fileStat.st_nlink); 
            printf(" %s", getpwuid(fileStat.st_uid)->pw_name);
            printf(" %s", getgrgid(fileStat.st_gid)->gr_name);
            printf(" %ld\t", fileStat.st_size); // no se como hacer para que no este todo chanfleado si no es con el tab
            char date[12];
            strftime(date, 13, "%b %d %H:%M", localtime(&(fileStat.st_ctim)));
            printf(" %s", date);
            printf(" %s\n", dir_entry_arr[i]->d_name);
        }

        if (closedir(directory) == 0)
            return EXIT_SUCCESS;

        return EXIT_FAILURE;
        }
        else {

        }

    fprintf(stderr, (argc == 1) ? "Error: Error al abrir el directorio corriente\n" : "Error: Error al abrir el directorio especificado\n");
    return EXIT_FAILURE;        
}
/*
stat 
dirent
opendir
readdir
S_ISREG  
DIR
*/
//stat(argv[1], &fileStat)
//printf("%s\n", dir_entry->d_name);