//
// Created by cendor on 07/05/24.
//

#include "../include/networking.h"

#include <sys/socket.h>

int do_socket(int domain, int type, int protocol)
{
    return socket(domain, type, protocol);
}
int do_bind();
int do_listen();

