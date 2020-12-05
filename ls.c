//
// Created by kautilya on 03/09/20.
//
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "headers.h"
#include "ls.h"
#include "dirent.h"

void print_group(gid_t gid){
    struct group *gname = getgrgid(gid);
    printf("%s",gname->gr_name);
}
void print_user(uid_t uid){
    struct passwd *pws;
    pws = getpwuid(uid);
    printf("%s", pws->pw_name);

}
void print_permissions(int mode){
    printf( (S_ISDIR(mode)) ? "d" : "-");
    printf( (mode & S_IRUSR) ? "r" : "-");
    printf( (mode & S_IWUSR) ? "w" : "-");
    printf( (mode & S_IXUSR) ? "x" : "-");
    printf( (mode & S_IRGRP) ? "r" : "-");
    printf( (mode & S_IWGRP) ? "w" : "-");
    printf( (mode & S_IXGRP) ? "x" : "-");
    printf( (mode & S_IROTH) ? "r" : "-");
    printf( (mode & S_IWOTH) ? "w" : "-");
    printf( (mode & S_IXOTH) ? "x" : "-");
}

void __ls(char *dir_temp, int a, int l, char * home_path)
{
    struct dirent *d;
    char dir[strlen(dir_temp) + strlen(home_path)+2];
    int home_length = strlen(home_path);
    if(dir_temp[0] == '~'){
        for (int i = 0; i < home_length; ++i) {
            dir[i] = home_path[i];
        }
        for (int i = 1; i < strlen(dir_temp); ++i) {
            dir[home_length+i-1] = dir_temp[i];
        }
        dir[home_length+strlen(dir_temp)-1] = '\0';
    } else{
        for (int i = 0; i < strlen(dir_temp); ++i) {
            dir[i] = dir_temp[i];
        }
        dir[strlen(dir_temp)] = '\0';
    }
    DIR *directory = opendir(dir);
    if(!directory){
        if(errno == ENOENT){
            printf("Directory does not exist");
        } else{
            printf("Unable to read the directory");
        }
        return;
    }
    int flag = 0;
    while ((d = readdir(directory)) != NULL){
        if(a == 0 && d->d_name[0] == '.')
            continue;
        flag = 1;
        if(l){
            struct stat dir_stat;
            stat(d->d_name,&dir_stat);
            print_permissions(dir_stat.st_mode);
            printf(" %2ld ",dir_stat.st_nlink);
            print_user(dir_stat.st_uid);
            printf(" ");
            print_group(dir_stat.st_gid);
            printf(" ");
            printf("%6ld ",dir_stat.st_size);
            char *last_modified_time = ctime(&dir_stat.st_mtime);
           last_modified_time[strlen(last_modified_time)-1-4-1-3] = '\0';
           last_modified_time+=4;
            printf("%s ", last_modified_time);
        }
        printf("%s\n", d->d_name);
    }
    closedir(directory);
}