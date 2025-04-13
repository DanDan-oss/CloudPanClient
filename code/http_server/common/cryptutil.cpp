#include "cryptutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

char* md5_bytes(const char *plaintext)
{

}

char* sha256_bytes(const char *plaintext)
{

}

char* des_encrypt_bytes(const char *plaintext, const char* keyBytes, const char *mode)
{

}

char* des_decrypt_bytes(const char *ciphertext, const char *keyBytes, const char *mode)
{

}

char* aes_encrypt_bytes(const char *plainBytes, const char *keyBytes, const char *ivBytes)
{

}

char* aes_decrypt_bytes(const char *ciphertext, const char *keyBytes, const char *ivBytes)
{

}

void md5_to_hex(const unsigned char *md5, char *hex) {
    for (int i = 0; i < 16; i++) {
        sprintf(hex + i * 2, "%02x", md5[i]);
    }
    hex[32] = '\0'; // MD5十六进制长度为32
}