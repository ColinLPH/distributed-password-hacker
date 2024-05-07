//
// Created by cendor on 01/05/24.
//

#include "parse_args.h"

#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

int parse_args(int argc, char *argv[], struct options *opts)
{
    int c;
    optind = 1;
    errno = 0;
    options_init(opts);
    while((c = getopt(argc, argv, ":s:a:l:p:")) != -1)
    {
        switch(c)
        {
            case 's':
                opts->ctrl_addr = strdup(optarg);
                break;
            case 'a':
                if(check_algo(optarg) == -1)
                {
                    strcpy(opts->err_msg, "Supported Algos: md5, sha1, sha256, sha384, sha512");
                    return -1;
                }
                snprintf(opts->algo, sizeof(opts->algo), "%s", optarg);
                break;
            case 'l':
                opts->pwd_len = str_to_int(optarg, opts);
                if(opts->pwd_len == -1)
                {
                    return -1;
                }
                break;
            case 'p':
                opts->port = (in_port_t) str_to_int(optarg, opts);
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

void options_init(struct options *opts)
{
    //assign default values
    strcpy(opts->algo, "sha512");
    opts->pwd_len = DEFAULT_PWD_LEN;
    opts->port = DEFAULT_PORT;
    opts->chunk_size = DEFAULT_CHK_SIZE;
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

int check_algo(char *algo)
{
    if(strcmp(algo, "md5") == 0)
    {
        return 0;
    }
    if(strcmp(algo, "sha-1") == 0)
    {
        return 0;
    }
    if(strcmp(algo, "sha-256") == 0)
    {
        return 0;
    }
    if(strcmp(algo, "sha-384") == 0)
    {
        return 0;
    }
    if(strcmp(algo, "sha-512") == 0)
    {
        return 0;
    }
    return -1;
}
