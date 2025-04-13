#ifndef _CRYPTUTIL_h
#define _CRYPTUTIL_h

#ifdef __cplusplus
extern "C"
{
#endif

extern char* md5_bytes(const char *plaintext);
extern char* sha256_bytes(const char *plaintext);

extern char* des_encrypt_bytes(const char *plaintext, const char* keyBytes, const char *mode);
extern char* des_decrypt_bytes(const char *ciphertext, const char *keyBytes, const char *mode);

extern char* aes_encrypt_bytes(const char *plainBytes, const char *keyBytes, const char *ivBytes);
extern char* aes_decrypt_bytes(const char *ciphertext, const char *keyBytes, const char *ivBytes);

void md5_to_hex(const unsigned char *md5, char *hex);

#ifdef __cplusplus
}
#endif

#endif