//
// Created by cendor on 01/05/24.
//

#include <stdlib.h>

#include "worker.h"



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

