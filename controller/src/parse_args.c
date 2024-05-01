//
// Created by cendor on 01/05/24.
//

#include "parse_args.h"

#include <stdio.h>
#include <stdlib.h>

#include "options.h"

int parse_args(int argc, char *argv[], struct options *opts)
{
    int c;
    optind = 1;
    errno = 0;
    while((c = getopt(argc, argv, ":s:a:l:p:")) != -1)
    {
        switch(c)
        {
            case 's':
                opts->ctrl_addr = strdup(optarg);
                break;
            case 'a':
                strcpy(opts->algo, optarg);
                break;
            case 'l':
                opts->max_pwd_len = strtol(optarg, NULL, 10);
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
                break;
            case 'p':
                opts->port = (in_port_t) strtol(optarg, NULL, 10);
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

    if(optind == argc)
    {
        //no worker addrs
        strcpy(opts->err_msg, "Must provide worker addresses\n");
        return -1;
    }

    opts->num_wkrs = argc - optind;
    opts->wkr_addrs = calloc(opts->num_wkrs, sizeof(char *));
    if(opts->wkr_addrs == NULL)
    {
        strcpy(opts->err_msg, "Worker addresses allocation failed\n");
        return -1;
    }

    for(int i = optind; i < argc; ++i)
    {
        opts->wkr_addrs[i-optind] = strdup(argv[i]);
        if(opts->wkr_addrs[i-optind] == NULL)
        {
            strcpy(opts->err_msg, "Worker address allocation failed\n");
            return -1;
        }
    }

    return 1;
}
