//
// Created by cendor on 01/05/24.
//

#include "controller.h"

#include <stdio.h>

#include "parse_args.h"

int run_controller(int argc, char *argv[], struct options *opts)
{

    if(parse_args(argc, argv, opts) == -1)
    {
        //print error, clean up
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }

    print_options(opts);
    clean_up(opts);

    return EXIT_SUCCESS;
}

void clean_up(struct options *opts)
{
    if(opts != NULL)
    {
        if(opts->ctrl_addr != NULL)
        {
            free(opts->ctrl_addr);
        }
        if(opts->wkr_addrs != NULL)
        {
            for(int i = 0; i < opts->num_wkrs; ++i)
            {
                if(opts->wkr_addrs[i] != NULL)
                {
                    free(opts->wkr_addrs[i]);
                }
            }
            free(opts->wkr_addrs);
        }

    }
}

void print_options(struct options *opts)
{
    printf("------------OPTIONS------------\n");
    printf("Controller IP: %s\nPort: %d\n", opts->ctrl_addr, opts->port);
    printf("Hash Algo: %s\nMax Password Len: %d\n", opts->algo, opts->max_pwd_len);
    printf("Number of Workers: %d\n", opts->num_wkrs);
    for(int i = 0; i < opts->num_wkrs; ++i)
    {
        printf("Worker[%d]: %s\n", i, opts->wkr_addrs[i]);
    }
    printf("------------OPTIONS------------\n");
}

void print_err(struct options *opts)
{
    printf("%s", opts->err_msg);
}
