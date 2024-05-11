//
// Created by cendor on 02/05/24.
//

#include "worker.h"


#define SET_SIZE 15
#define PKT_SIZE 512
const char set[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e'};

volatile __sig_atomic_t end_flag = 0;

void sigterm_handler(int signum)
{
    end_flag = 1;
}


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
    // if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &sockopt, sizeof(sockopt)) == -1)
    // {
    //     char msg[10];
    //     if(errno == EBADF)
    //     {
    //         strcpy(msg, "EBADF");
    //     }
    //     if(errno == EDOM)
    //     {
    //         strcpy(msg, "EDOM");
    //     }
    //     if(errno == EINVAL)
    //     {
    //         strcpy(msg, "EINVAL");
    //     }
    //     if(errno == EISCONN)
    //     {
    //         strcpy(msg, "EISCONN");
    //     }
    //     if(errno == ENOPROTOOPT)
    //     {
    //         strcpy(msg, "ENOPROTOOPT");
    //     }
    //     if(errno == ENOTSOCK)
    //     {
    //         strcpy(msg, "ENOTSOCK");
    //     }
    //     sprintf(opts->err_msg, "setsocketopt failed: %s\n", msg);
    //     print_err(opts);
    //     clean_up(opts);
    //     return EXIT_FAILURE;
    // }

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

    ssize_t rbytes;
    uint8_t buffer[PKT_SIZE];
    //read init
    struct init_packet init_pkt = {.type = UNDEF_TYPE, .algo = NULL, .hashed_str = NULL};
    rbytes = read(controllerfd, buffer, PKT_SIZE);
    if(rbytes == -1)
    {
        sprintf(opts->err_msg, "read failed\n");
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }
    deserialize_init(buffer, &init_pkt);
    if(init_pkt.type == UNDEF_TYPE)
    {
        sprintf(opts->err_msg, "init pkt failed\n");
        print_err(opts);
        clean_up(opts);
        return EXIT_FAILURE;
    }
    print_pkt(&init_pkt);

    //read assign
    //start thread_solve
    //stay reading for end type

    close(controllerfd);
    close(sockfd);
    clean_up(opts);
    return EXIT_SUCCESS;
    //read init, read assign, solve, raise signal if end received, request assign or send solution

}

void deserialize_init(uint8_t *buffer, struct init_packet *pkt)
{
    uint8_t type;
    size_t count = 0;
    memcpy(&type, &buffer[count], sizeof(type));
    count += sizeof(type);
    if(type == INIT_TYPE)
    {

    }
}

void *decrypt_thread(void *arg)
{

    pthread_exit(NULL);
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

void print_help(void)
{
    char msg[100];
    sprintf(msg, "Usage: <dir>/worker -w <worker ip> -p <port>");
    printf("%s\n", msg);
}

void print_pkt(void *ptr)
{
    struct init_packet *pkt = (struct init_packet *) ptr;
    switch(pkt->type)
    {
        case INIT_TYPE:
            printf("--------INIT PAKCET--------\n");
            printf("Algo: %s\n", pkt->algo);
            print_hash(pkt->algo, pkt->hashed_str);
            break;
        case ASSIGN_TYPE:
            printf("--------ASSIGN PAKCET--------\n");
            break;
        case END_TYPE:
            printf("--------END PAKCET--------\n");
            break;
        default:
            break;
    }
}

void print_hash(const char *algo, const unsigned char *hashed_str)
{
    printf("%s Hashed_str: ", algo);
    for(int i = 0; i < EVP_MD_size(EVP_get_digestbyname(algo)); i++) {
        printf("%02x", hashed_str[i]);
    }
    printf("\n");

}
