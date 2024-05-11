//
// Created by cendor on 01/05/24.
//

#include "controller.h"

#include <threads.h>

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

    struct pollfd poll_wkrs[opts->num_wkrs];
    for(int i = 0; i < opts->num_wkrs; ++i)
    {
        poll_wkrs[i].events = POLLIN;
        if((poll_wkrs[i].fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            sprintf(opts->err_msg, "socket failed\n");
            print_err(opts);
            clean_up(opts);
            return EXIT_FAILURE;
        }
        if (setsockopt(poll_wkrs[i].fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &sockopt, sizeof(sockopt)) == -1)
        {
            sprintf(opts->err_msg, "setsocketopt failed\n");
            print_err(opts);
            clean_up(opts);
            return EXIT_FAILURE;
        }
        if(inet_pton(AF_INET, opts->wkr_addrs[i], &ipv4_addr.sin_addr) != 1)
        {
            sprintf(opts->err_msg, "ient_pton failed\n");
            print_err(opts);
            clean_up(opts);
            return EXIT_FAILURE;
        }
        ipv4_addr.sin_family = AF_INET;
        ipv4_addr.sin_port = htons(opts->port);
        if(connect(poll_wkrs[i].fd, (struct sockaddr *) &ipv4_addr, sizeof(ipv4_addr)) == -1)
        {
            sprintf(opts->err_msg, "connection to %s failed\n", opts->wkr_addrs[i]);
            print_err(opts);
            clean_up(opts);
            return EXIT_FAILURE;
        }
        printf("Connected to worker: %s\n", opts->wkr_addrs[i]);
    }

    struct init_packet init_pkt = {INIT_TYPE, opts->algo, hashed_str};
    uint8_t *buffer = calloc(1, sizeof(init_pkt));
    serialize_pkt(buffer, &init_pkt);
    for(int i = 0; i < opts->num_wkrs; ++i)
    {
        //send init pkts to all workers
        write(poll_wkrs[i].fd, buffer, sizeof(init_pkt));
    }
    //send assign pkts to all workers
    //start polling for responses

    for (int i = 0; i < opts->num_wkrs; i++) {
        if (poll_wkrs[i].fd != -1) {
            close(poll_wkrs[i].fd);
        }
    }
    clean_up(opts);
    return EXIT_SUCCESS;
}

void serialize_pkt(uint8_t *buffer, void *pkt)
{
    struct init_packet * temp = pkt;
    size_t count = 0;
    switch(temp->type)
    {
        case INIT_TYPE:
            memcpy(&buffer[count], &temp->type, sizeof(temp->type));
            count += sizeof(temp->type);
            memcpy(&buffer[count], &temp->algo, sizeof(temp->algo));
            count += sizeof(temp->algo);
            memcpy(&buffer[count], &temp->hashed_str, sizeof(temp->hashed_str));
            break;
        case ASSIGN_TYPE:
            struct assignment_packet *assign = pkt;
            memcpy(&buffer[count], &assign->type, sizeof(assign->type));
            count += sizeof(assign->type);
            memcpy(&buffer[count], &assign->lower_bound, sizeof(assign->lower_bound));
            count += sizeof(assign->lower_bound);
            memcpy(&buffer[count], &assign->upper_bound, sizeof(assign->upper_bound));
            break;
        case END_TYPE:
            struct end_packet *end = pkt;
            memcpy(&buffer[count], &end->type, sizeof(end->type));
            break;
        default:
            printf("Bad packet type\n");
            break;
    }
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

