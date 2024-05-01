//
// Created by cendor on 01/05/24.
//

#include <stdio.h>
#include <string.h>

#include "controller.h"

void print_help(void);

int main(int argc, char *argv[])
{
    struct options *opts;

    if(argc == 1)
    {
        print_help();
        return EXIT_FAILURE;
    }

    opts = calloc(1, sizeof(*opts));
    return run_controller(argc, argv, opts);
}

void print_help(void)
{
    char msg[500];
    sprintf(msg, "Usage: <dir>/controller -s <controller ip> -p <port> ");
    strcat(msg, "-a <hash algo>  -l <max pwd len> <worker ip addresses...>");
    printf("%s\n", msg);
}
