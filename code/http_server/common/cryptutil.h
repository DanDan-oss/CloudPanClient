/*********************************************************
 *  des.h
 *  用户使用des算法头文件
 *	
 *********************************************************/
#ifndef _OPENDESS_H_
#define _OPENDESS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned int count[2];
    unsigned int state[4];
    unsigned char buffer[64];   
}MD5_CTX;

#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
#define FF(a,b,c,d,x,s,ac) \
          { \
          a += F(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define GG(a,b,c,d,x,s,ac) \
          { \
          a += G(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define HH(a,b,c,d,x,s,ac) \
          { \
          a += H(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define II(a,b,c,d,x,s,ac) \
          { \
          a += I(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }

extern char* base64_encode(const unsigned char *bindata, int binlength, char *base64);
extern int base64_decode(const char *base64, unsigned char* bindata);

extern int desEncryptText(unsigned char *pInData, int nInDataLen, unsigned char *pOutData, int *pOutDataLen);
extern int desEncryptTextRaw(unsigned char *pInData, int nInDataLen, unsigned char *pOutData, int *pOutDataLen);
extern int desDecryptText(unsigned char *pInData, int nInDataLen, unsigned char *pOutData, int *pOutDataLen);
extern int desDecryptTextRaw(unsigned char *pInData, int nInDataLen, unsigned char *pOutData, int *pOutDataLen);

extern void md5_to_hex(const unsigned char *md5, char *hex);
extern void md5Init(MD5_CTX *context);
extern void md5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
extern void md5Final(MD5_CTX *context,unsigned char digest[16]);
extern void md5Encode(unsigned char *output,unsigned int *input,unsigned int len);
extern void md5Decode(unsigned int *output,unsigned char *input,unsigned int len);
extern void md5Transform(unsigned int state[4],unsigned char block[64]);

#ifdef __cplusplus
}
#endif

#endif

