//
// Created by kautilya on 10/09/20.
//

#include <unistd.h>
#include "string.h"
#include "stdio.h"
#include "signal.h"

int main(int argc, char **tokens){
    if(tokens[1]){
        int n = 0;
        for (int j = 0; j < strlen(tokens[1]); ++j) {
            n = (int) (n * 10 + (tokens[1][j] - '0'));
        }
        sleep(n);
    }
    return 0;
}
