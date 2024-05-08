//
// Created by cendor on 01/05/24.
//

#include "controller.h"

#define SET_SIZE 15
const char set[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e'};

int run_controller(int argc, char *argv[], struct options *opts)
{
    int sockfd;
    int sockopt = 1;
    struct sockaddr_in ipv4_addr;
    int addr_len = sizeof(ipv4_addr);

    if(parse_args(argc, argv, opts) == -1)
    {
        //print error, clean up
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }
    print_options(opts);

    char pwd[opts->pwd_len+1];
    memset(pwd, 0, sizeof(pwd));
    generate_pwd(pwd, opts->pwd_len);                    //generate random password
    printf("Password: %s\n", pwd);

    unsigned char hashed_str[EVP_MAX_MD_SIZE];
    generate_hash(opts->algo, pwd, hashed_str);     //generate hash
    print_hash(opts->algo, hashed_str);

    //separate possible pwds into chunks
    char chunks[SET_SIZE][opts->pwd_len];
    for(int i = 0; i < SET_SIZE; ++i)
    {
        for(int j = 0; j < opts->pwd_len; ++j)
        {
            chunks[i][j] = set[i];
        }
    }

    //connect with workers, send init, send assign chunks
    //socket, bind, connect
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        sprintf(opts->err_msg, "socket failed\n");
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &sockopt, sizeof(sockopt)) == -1) {
        sprintf(opts->err_msg, "setsocketopt failed\n");
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }

    if(inet_pton(AF_INET, opts->ctrl_addr, &ipv4_addr.sin_addr) != 1)
    {
        sprintf(opts->err_msg, "ient_pton failed\n");
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }
    ipv4_addr.sin_family = AF_INET;
    ipv4_addr.sin_port = htons(opts->port);

    if(connect(sockfd, (struct sockaddr *) &ipv4_addr, sizeof(ipv4_addr)) == -1)
    {
        sprintf(opts->err_msg, "connect failed\n");
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }
    printf("Connected to worker\n");
    //wait for replies with pollfd, send assigns again
    //coordinate tasks until completion
    close(sockfd);
    clean_up(opts);
    return EXIT_SUCCESS;
}

void generate_pwd(char *pwd, const int len)
{
    srand(time(NULL));
    for(int i = 0; i < len; ++i)
    {
        const int ret = rand()%SET_SIZE;
        pwd[i] = set[ret];
    }
    pwd[len] = '\0';
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
        free(opts);
    }
}

void print_options(struct options *opts)
{
    printf("------------OPTIONS------------\n");
    printf("Controller IP: %s\nPort: %d\n", opts->ctrl_addr, opts->port);
    printf("Hash Algo: %s\nPassword Len: %d\n", opts->algo, opts->pwd_len);
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

void print_hash(const char *algo, const unsigned char *hashed_str)
{
    printf("%s Hashed_str: ", algo);
    for(int i = 0; i < EVP_MD_size(EVP_get_digestbyname(algo)); i++) {
        printf("%02x", hashed_str[i]);
    }
    printf("\n");

}

