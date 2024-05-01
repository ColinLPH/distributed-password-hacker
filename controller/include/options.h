//
// Created by cendor on 01/05/24.
//

#ifndef OPTIONS_H
#define OPTIONS_H

#include <netinet/in.h>

struct options {
    char algo[8];
    int max_pwd_len;
    in_port_t port;
    int num_wkrs;
    char err_msg[50];
    char *ctrl_addr;
    char **wkr_addrs;
};

#endif //OPTIONS_H
