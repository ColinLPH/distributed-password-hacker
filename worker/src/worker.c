//
// Created by cendor on 02/05/24.
//

#include "worker.h"

#define SET_SIZE 15
const char set[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e'};

int run_worker(int argc, char *argv[], struct options *opts) {
    int sockfd, controllerfd;
    int sockopt = 1;
    struct sockaddr_in ipv4_addr;
    int add_len = sizeof(ipv4_addr);

    if(parse_args(argc, argv, opts) == -1)
    {
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }
    print_options(opts);
    //socket, bind, listen, accept
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

    if(inet_pton(AF_INET, opts->wkr_addr, &ipv4_addr.sin_addr) != 1)
    {
        sprintf(opts->err_msg, "ient_pton failed\n");
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }
    ipv4_addr.sin_family = AF_INET;
    ipv4_addr.sin_port = htons(opts->port);

    if(bind(sockfd, (struct sockaddr *) &ipv4_addr, sizeof(ipv4_addr)) == -1)
    {
        sprintf(opts->err_msg, "bind failed\n");
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }

    if(listen(sockfd, 1) == -1)
    {
        sprintf(opts->err_msg, "listen failed\n");
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }

    if((controllerfd = accept(sockfd, (struct sockaddr *) &ipv4_addr, (socklen_t *)&add_len) == -1))
    {
        sprintf(opts->err_msg, "accept failed\n");
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }

    printf("Controller connected\n");

    close(controllerfd);
    close(sockfd);
    return EXIT_SUCCESS;
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
