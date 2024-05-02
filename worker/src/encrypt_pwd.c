//
// Created by cendor on 02/05/24.
//

#include "encrypt_pwd.h"

#include <string.h>

void generate_hash(const char *algo, const char *input, unsigned char *output)
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned int md_len;

    OpenSSL_add_all_algorithms();
    md = EVP_get_digestbyname(algo);

    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, input, strlen(input));
    EVP_DigestFinal_ex(mdctx, output, &md_len);
    EVP_MD_CTX_free(mdctx);
}
