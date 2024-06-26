//
// Created by cendor on 02/05/24.
//

#ifndef OPTIONS_H
#define OPTIONS_H

#include <netinet/in.h>

#define INIT_TYPE 0
#define ASSIGN_TYPE 1
#define END_TYPE 2
#define UNDEF_TYPE 3

#define DEFAULT_PORT 5000

struct options
{
    in_port_t port;
    char err_msg[50];
    char algo[8];
    char *wkr_addr;
};

//sent by controller to inform worker which hash to use and deliver the hashed_str
struct init_packet
{
    uint8_t type;
    uint8_t *algo;
    unsigned char *hashed_str;
};

//sent by controller to assign a chunk of passwords for the worker to try
struct assignment_packet
{
    uint8_t type;
    char *lower_bound;
    char *upper_bound;
};

//sent by the controller to tell the worker to stop
struct end_packet
{
    uint8_t type;
};

//sent by the worker to indicate that the chunk has been finished and/or the answer has been found
struct reply_packet
{
    int found;
    char *pwd;
};

#endif //OPTIONS_H
