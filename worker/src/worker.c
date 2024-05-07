//
// Created by cendor on 02/05/24.
//

#include "worker.h"

#define SET_SIZE 15
const char set[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e'};

int run_worker(int argc, char *argv[], struct options *opts)
{
    if(parse_args(argc, argv, opts) == -1)
    {
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }
    print_options(opts);
    //socket, bind, listen, accept
    //read init, read assign, solve, raise signal if end received, request assign or send solution

}

void clean_up(struct options *opts)
{
    if(opts)
    {
        if(opts->wkr_addr != NULL)
        {
            free(opts->wkr_addr);
        }
        free(opts);
    }
}

void print_options(struct options *opts)
{
    printf("------------OPTIONS------------\n");
    printf("Worker IP: %s\nPort: %d\n", opts->wkr_addr, opts->port);
    printf("------------OPTIONS------------\n");
}

void print_err(struct options *opts)
{
    printf("%s", opts->err_msg);
}
