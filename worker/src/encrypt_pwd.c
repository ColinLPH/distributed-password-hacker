//
// Created by cendor on 02/05/24.
//

#include "encrypt_pwd.h"

#include <string.h>

void generate_hash(const char *algo, const char *input, unsigned char *output)
{
    EVP_MD_CTX *mdctx = NULL;
    const EVP_MD *md = NULL;
    unsigned int md_len = 0;

    OpenSSL_add_all_digests();

    md = EVP_get_digestbyname(algo);
    if (!md) {
        fprintf(stderr, "Error: Unknown digest algorithm '%s'\n", algo);
        return;
    }

    mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        fprintf(stderr, "Error: Failed to create EVP_MD_CTX\n");
        return;
    }

    if (EVP_DigestInit_ex(mdctx, md, NULL) != 1) {
        fprintf(stderr, "Error: EVP_DigestInit_ex failed\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    if (EVP_DigestUpdate(mdctx, input, strlen(input)) != 1) {
        fprintf(stderr, "Error: EVP_DigestUpdate failed\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    if (EVP_DigestFinal_ex(mdctx, output, &md_len) != 1) {
        fprintf(stderr, "Error: EVP_DigestFinal_ex failed\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    EVP_MD_CTX_free(mdctx);

    printf("Hash Length: %u bytes\n", md_len);
}
