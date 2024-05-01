//
// Created by cendor on 01/05/24.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdlib.h>
#include "options.h"

int run_controller(int argc, char *argv[], struct options *opts);

void clean_up(struct options *opts);
void print_err(struct options *opts);
void print_options(struct options *opts);

#endif //CONTROLLER_H
