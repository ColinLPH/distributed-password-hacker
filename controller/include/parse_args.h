//
// Created by cendor on 01/05/24.
//

#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include <errno.h>
#include <getopt.h>
#include <string.h>
#include "options.h"

int parse_args(int argc, char *argv[], struct options *opts);

#endif //PARSE_ARGS_H
