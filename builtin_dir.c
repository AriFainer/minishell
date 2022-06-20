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
#include <stdbool.h>

#include "minish.h"
#include "wrappers.h"

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

    bool filter_exists = false;
    char *filter;
    DIR *directory;
    struct dirent *dir_entry_arr[1024]; //aca todavia cte magica
    int dirent_arr_size = 0;
    char *search_dir;
    char dir_entry_path[MAXCWD];
    
    if (argc == 1) {
        search_dir = ".";
    }
    else if (argc == 2) {
        search_dir = argv[1];
    }
    else {  
        filter_exists = true;
        search_dir = argv[1];
        filter = argv[2];
    }

    directory = opendir(search_dir);
    if (directory == NULL) {
        if(argc != 2){
            fprintf(stderr, "Error: Error al abrir el directorio especificado\n");
            return EXIT_FAILURE;
        }
        // Filtrando en el directorio corriente
        filter_exists = true;
        filter = argv[1];
        search_dir = ".";
        directory = opendir(search_dir);
    }

    struct dirent *dir_entry;
    while ((dir_entry = readdir(directory)) != NULL) {
        if((!filter_exists && dir_entry->d_name[0] == '.') || (filter_exists && strstr(dir_entry->d_name, filter) == NULL)){
            continue; // si arranca con "." lo ignoro como en el 'ls -l'
        }
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
        strcpy(dir_entry_path, search_dir);
        strcat(dir_entry_path,"/");
        strcat(dir_entry_path, dir_entry_arr[i]->d_name);
        if(stat(dir_entry_path, &fileStat) < 0) { // No se logra acceder a alguno de los archivos, no se muestra
            printf("Error: Error al obtener la informacion de \"%s\"\n", dir_entry_arr[i]->d_name);
            continue;
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
        printf(" %ld\t", fileStat.st_size); 
        char date[12];
        strftime(date, 13, "%b %d %H:%M", localtime(&(fileStat.st_ctim.tv_sec)));
        printf(" %s", date);
        printf(" %s\n", dir_entry_arr[i]->d_name);
    }

    if (closedir(directory) == 0)
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}