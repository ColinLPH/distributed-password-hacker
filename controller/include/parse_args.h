//
// Created by cendor on 01/05/24.
//

#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "structs.h"

int parse_args(int argc, char *argv[], struct options *opts);
int check_algo(char *algo);
int str_to_int(char *str, struct options *opts);
void options_init(struct options *opts);

#endif //PARSE_ARGS_H
