//
// Created by cendor on 02/05/24.
//

#ifndef WORKER_H
#define WORKER_H


#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <openssl/evp.h>
#include <signal.h>
#include "parse_args.h"
#include "options.h"

int run_worker(int argc, char *argv[], struct options *opts);

void serialize_pkt();
void deserialize_init(uint8_t *buffer, struct init_packet *pkt);
void *decrypt_thread(void *arg);

void clean_up(struct options *opts);
void print_options(struct options *opts);
void print_err(struct options *opts);
void print_help(void);
void print_pkt(void *ptr);
void print_hash(const char *algo, const unsigned char *hashed_str);

#endif //WORKER_H
