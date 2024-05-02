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

#include "options.h"
int parse_args(int argc, char *argv[], struct options *opts);
int check_algo(char *algo);

#endif //PARSE_ARGS_H
