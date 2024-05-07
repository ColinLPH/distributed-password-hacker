//
// Created by cendor on 02/05/24.
//

#include "parse_args.h"

int parse_args(int argc, char *argv[], struct options *opts)
{
    int c;
    optind = 1;
    errno = 0;
    opts->port = 5000;
    opts->wkr_addr = NULL;
    while((c = getopt(argc, argv, ":p:w:")) != -1)
    {
        switch(c)
        {
            case 'p':
                opts->port = (in_port_t) str_to_int(optarg, opts);
                break;
            case 'w':
                opts->wkr_addr = strdup(optarg);
                break;
            case ':':
                sprintf(opts->err_msg, "Flag -%c requires an argument\n", c);
                return -1;
            case '?':
                sprintf(opts->err_msg, "Unknown option -%c\n", c);
                return -1;
            default:
                printf("Why am I here\n");
                break;
        }
    }
}

int str_to_int(char *str, struct options *opts)
{
    errno = 0;
    int ret = strtol(str, NULL, 10);
    if(errno == EINVAL)
    {
        strcpy(opts->err_msg, "pwd len conversion failed\n");
        return -1;
    }
    if(errno == ERANGE)
    {
        strcpy(opts->err_msg, "pwd len outside of range\n");
        return -1;
    }
    return ret;
}
