//
// Created by cendor on 01/05/24.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdlib.h>
#include "encrypt_pwd.h"
#include "options.h"
#include "parse_args.h"


int run_controller(int argc, char *argv[], struct options *opts);
void generate_pwd(char *pwd, const int len);

void clean_up(struct options *opts);
void print_err(struct options *opts);
void print_options(struct options *opts);
void print_hash(const char *algo, const unsigned char *hashed_str);

#endif //CONTROLLER_H
