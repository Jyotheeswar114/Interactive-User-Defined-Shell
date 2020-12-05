//
// Created by kautilya on 03/09/20.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "parse.c"
#include "string.h"
#include "stdlib.h"

int main(int n, char **args)
{
    printf("pid --- %s\n",args[1]);
    char path[100],stats[1024];
    sprintf(path,"/proc/%s/stat",args[1]);
    int fd = open(path,O_RDONLY);
    read(fd, stats, 1024);
    stats[strlen(stats) - 1] = '\0';
    char **tokens = parse(stats);
    printf("Process Status --- %s\n", tokens[2]);
    printf("memory --- %s\n", tokens[22]);
    sprintf(path, "/proc/%s/exe", args[1]);
    char buff[100];
    int len = readlink(path, buff, 100 - 1);
    buff[len] = '\0';
    printf("Executable Path -- %s\n", buff);
    free(tokens);
    return 0;
}
