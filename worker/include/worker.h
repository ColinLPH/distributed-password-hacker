//
// Created by cendor on 02/05/24.
//

#ifndef WORKER_H
#define WORKER_H


#include <arpa/inet.h>
#include <unistd.h>
#include "parse_args.h"
#include "options.h"

int run_worker(int argc, char *argv[], struct options *opts);

void clean_up(struct options *opts);
void print_options(struct options *opts);
void print_err(struct options *opts);

#endif //WORKER_H
