//
// Created by cendor on 01/05/24.
//

#include <stdio.h>
#include <stdlib.h>

#include "worker.h"

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
    return run_worker(argc, argv, opts);
}

void print_help(void)
{
    char msg[100];
    sprintf(msg, "Usage: <dir>/worker -s <worker ip> -p <port>");
    printf("%s\n", msg);
}
