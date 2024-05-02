//
// Created by cendor on 02/05/24.
//

#ifndef SHA256_H
#define SHA256_H

#include <openssl/evp.h>

void generate_hash(const char *algo, const char *input, unsigned char *output);

#endif //SHA256_H
