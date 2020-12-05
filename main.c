#include <wait.h>
#include "headers.h"
#include "prompt.h"
#include "parse.h"
#include "ls.h"
#include "signal.h"

extern int errno;
char home_path[1024];
void handler(int num){
    write(1, "\nChild ended!\n", 14);
    prompt(home_path);
}

int main() {

    signal(SIGUSR1, handler);
    char command[100],command_copy[100];
    getcwd(home_path, 1024);
    int i = 0, args_n;
    while (i == 0){
        int child_pid;
        prompt(home_path);
        uid_t parent_id = getpid();

        // reading command
        fgets(command, 100, stdin);
        if(command[0] == '\n')
            continue;
        command[strlen(command) - 1] = '\0';
        strcpy(command_copy, command);

        char **tokens = parse(command);// parsing command
        args_n = 0;
        while(tokens[args_n])
            args_n++;

        if(strcmp(tokens[0], "cd") == 0) {
            int status;
            if(!tokens[1])
                status = chdir(home_path);
            else if (strcmp(tokens[1], "~") == 0) {
                char new_path[strlen(home_path) + strlen(tokens[1]) + 1];
                strcpy(new_path, home_path);
                strcpy(new_path + strlen(home_path), tokens[1] + 1);
                status = chdir(new_path);
            } else {
                status = chdir(tokens[1]);
            }
            if (status == -1){
                printf("There is no directory like that\n");
                continue;
            }
        }

        else if (strcmp(tokens[0] , "echo" ) == 0){

            if(tokens[1]) {
                if(tokens[1][0] == '\"'){
                    int i = 0;
                    while (command_copy[i] != '\"')
                        i++;
                    command_copy[strlen(command_copy) - 1] = '\0';
                    printf("%s",command_copy+i+1);
                }
                else {
                    int i = 2;

                    printf("%s", tokens[1]);
                    while (tokens[i]) {
                        printf(" %s", tokens[i]);
                        i++;
                    }
                }
            }
            printf("\n");
        }

        else if (strcmp(tokens[0], "ls") == 0){
            if(args_n == 1)
                __ls(".",0,0,home_path);
            else if(args_n == 2)
            {
                if(tokens[1][0] == '-'){
                    int a = 0, l = 0;
                    char *c =(char *) (tokens[1] + 1);
                    while (*c){
                        if(*c == 'a') a = 1;
                        else if (*c == 'l') l = 1;
                        else{
                            perror("Unknown Option Found");
                            continue;
                        }
                        c++;
                    }
                    __ls(".",a,l, home_path);
                } else
                    __ls(tokens[1], 0, 0, home_path);
            }
            else if (args_n == 3){
                if(tokens[1][0] != '-') {
                    perror("Incorrect Usage Error");
                    continue;
                }
                int a = 0, l = 0;
                char *c =(char *) (tokens[1] + 1);
                while (*c){
                    if(*c == 'a') a = 1;
                    else if (*c == 'l') l = 1;
                    else{
                        perror("Unknown Option Found");
                        continue;
                    }
                    c++;
                }
                free(c);
                __ls(tokens[2],a,l,home_path);
            }
        }
        else if (strcmp(tokens[0], "pwd") == 0){
            char path[1024];
            getcwd(path, 1024);
            printf("%s\n",path);
        }
        else if (strcmp(tokens[0], "exit") == 0){
            free(tokens);
            printf("Thanks for coming. \nHave a good day buddy. :)");
            return 0;
        }

        else{
            child_pid = fork();
            if(child_pid == 0){
                pid_t _pid = fork();
                if(_pid == 0) {
                    if (strcmp(tokens[0], "pinfo") == 0) {
                        char *args[10];
                        args[0] = "./pinfo";
                        if (tokens[1]) {
                            args[1] = tokens[1];
                        } else {
                            char temp[100];
                            sprintf(temp, "%d", parent_id);
                            args[1] = temp;
                        }
                        args[2] = NULL;
                        if (execvp(args[0], args) == -1) {
                            perror("Program");
                        };
                    } else if (strcmp(tokens[0], "sleep") == 0) {
                        char *args[10];
                        args[0] = "./sleep";
                        args[1] = tokens[1];
                        if (strcmp(tokens[args_n - 1], "&") == 0) {
                            args[2] = "&";
                            args[3] = NULL;
                        } else {
                            args[2] = NULL;
                        }
                        if (execvp(args[0], args) == -1) {
                            perror("Program");
                        }
                    }
                }
                else{
                    int st;
                    waitpid(_pid, &st,0);
                    if (strcmp(tokens[args_n - 1], "&") == 0) {
                        printf("\n%s with pid %d is exited normally\n", tokens[0], getpid());
                        prompt(home_path);
                    }
                    exit(0);
                }
            }
            else{

                if (strcmp(tokens[args_n-1], "&") == 0){
                    //printf("store in background list\n");
                } else{
                    int status;
                    waitpid(child_pid,&status,0);
                }
            }
        }
        free(tokens);
    }
    return 0;
}
