//
// Created by cendor on 02/05/24.
//

#ifndef OPTIONS_H
#define OPTIONS_H

#include <netinet/in.h>

struct options
{
    in_port_t port;
    char err_msg[50];
    char *wkr_addr;
};


#endif //OPTIONS_H
