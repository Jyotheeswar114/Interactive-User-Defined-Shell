//
// Created by kautilya on 01/09/20.
//
#include "prompt.h"

struct utsname user_name;


void prompt(char *home_path)
{

       int u = uname(&user_name);
       char path[1024] = "";
       getcwd(path, 1024);
       char temp[1024];
        sprintf(temp,"<%s@%s:",user_name.nodename, user_name.sysname);
        write(1,temp,strlen(temp));


       if(strlen(home_path) > strlen(path)) {
           sprintf(temp,"%s>", path);
            write(1,temp,strlen(temp));
       }
       else{
           int flag = 1;
           for (int i = 0; i < strlen(home_path); ++i) {
               if(home_path[i] != path[i]){

                   flag = 0;
                   break;
               }
           }


           if(flag){
                sprintf(temp,"~%s>",path + strlen(home_path));
           } else
               sprintf(temp,"%s>",path);
           write(1, temp, strlen(temp));
       }
}
