#include "cryptutil.h"
#include "openssl/des.h"
#include "openssl/aes.h"
#include "openssl/md5.h"
#include "openssl/sha.h"

QString CryptUtil::DEFAULT_DES_KEY = "12345678";//block size 8
QString CryptUtil::DEFAULT_AES_KEY = "1234567812345678";//block size 16
QString CryptUtil::DEFAULT_AES_IV = "1234567812345678";//block size 16

QString CryptUtil::md5Text(const QString &plaintext)
{
    if (plaintext.isEmpty()) {
        return plaintext;
    }
    return md5Bytes(plaintext.toUtf8()).toHex();
}

QByteArray CryptUtil::md5Bytes(const QByteArray &plainBytes)
{
    if (plainBytes.isEmpty()) {
        return plainBytes;
    }
    QByteArray output(MD5_DIGEST_LENGTH, 0);
    MD5((const unsigned char*)plainBytes.data(), plainBytes.size(), (unsigned char*)output.data());
    return output;
}

QString CryptUtil::sha256Text(const QString &plaintext)
{
    if (plaintext.isEmpty()) {
        return plaintext;
    }
    return sha256Bytes(plaintext.toUtf8()).toHex();
}

QByteArray CryptUtil::sha256Bytes(const QByteArray &plainBytes)
{
    if (plainBytes.isEmpty()) {
        return plainBytes;
    }
    QByteArray output(SHA256_DIGEST_LENGTH, 0);
    SHA256((const unsigned char*)plainBytes.data(), plainBytes.size(), (unsigned char*)output.data());
    return output;
}

QString CryptUtil::desEncryptText(const QString &plaintext, const QString &key, const QString &mode)
{
    if (plaintext.isEmpty()) {
        return plaintext;
    }
    QByteArray cipherBytes = desEncryptBytes(plaintext.toUtf8(), key.toUtf8(), mode);
    return cipherBytes.toHex();
}

QString CryptUtil::desDecryptText(const QString &ciphertext, const QString &key, const QString &mode)
{
    if (ciphertext.isEmpty()) {
        return ciphertext;
    }
    QByteArray cipherBytes = QByteArray::fromHex(ciphertext.toUtf8());
    return desDecryptBytes(cipherBytes, key.toUtf8(), mode);
}

QByteArray CryptUtil::desEncryptBytes(const QByteArray &plainBytes, const QByteArray &keyBytes, const QString &mode) {
    if (plainBytes.isEmpty()) {
        return plainBytes;
    }

    DES_cblock ivec;
    if (keyBytes.size() != 8) {
        memset((char*)&ivec, 0, sizeof(ivec));
    } else {
        memcpy(ivec, keyBytes.constData(), 8);
    }

    DES_cblock keyBlock;
    memcpy(keyBlock, keyBytes.constData(), 8);

    DES_key_schedule keySchedule;
    DES_set_key_unchecked(&keyBlock, &keySchedule);

    //Padding8
    QByteArray alignPlainBytes(plainBytes);
    if (alignPlainBytes.size() % 8 != 0) {
        alignPlainBytes.append(8 - alignPlainBytes.size() % 8, 0);
    }

    QByteArray cipherBytes;
    if (mode.contains("CBC", Qt::CaseInsensitive)) {
        cipherBytes.resize(alignPlainBytes.size());
        DES_ncbc_encrypt(reinterpret_cast<const unsigned char *>(alignPlainBytes.constData()),
                         reinterpret_cast<unsigned char *>(cipherBytes.data()),
                         alignPlainBytes.size(),
                         &keySchedule,
                         &ivec,
                         DES_ENCRYPT);
    } else {//ECB
        int len = alignPlainBytes.length();
        for (int i = 0; i < len; i += 8) {
            const_DES_cblock in;
            memcpy(in, alignPlainBytes.mid(i, 8).constData(), 8);

            DES_cblock out;
            DES_ecb_encrypt(&in, &out, &keySchedule, DES_ENCRYPT);

            cipherBytes.append(QByteArray::fromRawData((char *)out, 8));
        }
    }

    return cipherBytes.constData();
}

QByteArray CryptUtil::desDecryptBytes(const QByteArray &cipherBytes, const QByteArray &keyBytes, const QString &mode) {
    if (cipherBytes.isEmpty()) {
        return cipherBytes;
    }

    DES_cblock ivec;
    if (keyBytes.size() != 8) {
        memset((char*)&ivec, 0, sizeof(ivec));
    } else {
        memcpy(ivec, keyBytes.constData(), 8);
    }

    DES_cblock keyBlock;
    memcpy(keyBlock, keyBytes.constData(), 8);

    DES_key_schedule keySchedule;
    DES_set_odd_parity(&keyBlock);
    DES_set_key_unchecked(&keyBlock, &keySchedule);

    QByteArray plainBytes;
    if (mode.contains("CBC", Qt::CaseInsensitive)) {
        plainBytes.resize(cipherBytes.size());
        DES_ncbc_encrypt(reinterpret_cast<const unsigned char *>(cipherBytes.constData()),
                         reinterpret_cast<unsigned char *>(plainBytes.data()),
                         cipherBytes.size(),
                         &keySchedule,
                         &ivec,
                         DES_DECRYPT);
    } else  {//ECB
        //Padding8
        QByteArray alignCipherBytes(cipherBytes);
        if (cipherBytes.size() % 8 != 0) {
            alignCipherBytes.append(8 - cipherBytes.size() % 8, 0);
        }

        int len = alignCipherBytes.length();
        for (int i = 0; i < len; i += 8) {
            const_DES_cblock in;
            memcpy(in, alignCipherBytes.mid(i, 8).constData(), 8);

            DES_cblock out;
            DES_ecb_encrypt(&in, &out, &keySchedule, DES_DECRYPT);

            plainBytes.append(QByteArray::fromRawData((char *)out, 8));
        }
    }

    return plainBytes.constData();
}

QString CryptUtil::aesEncryptText(const QString &plaintext, const QString &key, const QString &iv)
{
    if (plaintext.isEmpty()) {
        return plaintext;
    }
    QByteArray cipherBytes = aesEncryptBytes(plaintext.toUtf8(), key.toUtf8(), iv.toUtf8());
    return cipherBytes.toHex();
}

QString CryptUtil::aesDecryptText(const QString &ciphertext, const QString &key, const QString &iv)
{
    if (ciphertext.isEmpty()) {
        return ciphertext;
    }
    QByteArray cipherBytes = QByteArray::fromHex(ciphertext.toUtf8());
    return aesDecryptBytes(cipherBytes, key.toUtf8(), iv.toUtf8());
}

QByteArray CryptUtil::aesEncryptBytes(const QByteArray &plainBytes, const QByteArray &keyBytes, const QByteArray &ivBytes) {
    if (plainBytes.isEmpty()) {
        return plainBytes;
    }

    AES_KEY aesKey;
    AES_set_encrypt_key(reinterpret_cast<const unsigned char*>(keyBytes.constData()),
                        8 * keyBytes.length(),
                        &aesKey);

    QByteArray ivBytes2(ivBytes);

    //ZeroPadding
    QByteArray alignPlainBytes(plainBytes);
    if (alignPlainBytes.size() % AES_BLOCK_SIZE != 0) {
        alignPlainBytes.append(AES_BLOCK_SIZE - alignPlainBytes.size() % AES_BLOCK_SIZE, 0);
    }

    QByteArray cipherBytes;
    cipherBytes.resize(alignPlainBytes.length());

    AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(alignPlainBytes.constData()),
                    reinterpret_cast<unsigned char *>(cipherBytes.data()),
                    cipherBytes.size(),
                    &aesKey,
                    reinterpret_cast<unsigned char *>(ivBytes2.data()),
                    AES_ENCRYPT);

    return cipherBytes.constData();
}

QByteArray CryptUtil::aesDecryptBytes(const QByteArray &cipherBytes, const QByteArray &keyBytes, const QByteArray &ivBytes) {
    if (cipherBytes.isEmpty()) {
        return cipherBytes;
    }

    AES_KEY aesKey;
    AES_set_decrypt_key(reinterpret_cast<const unsigned char*>(keyBytes.constData()),
                        8 * keyBytes.length(),
                        &aesKey);

    QByteArray ivBytes2(ivBytes);

    QByteArray plainBytes;
    plainBytes.resize(cipherBytes.length());

    AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(cipherBytes.constData()),
                    reinterpret_cast<unsigned char *>(plainBytes.data()),
                    cipherBytes.size(),
                    &aesKey,
                    reinterpret_cast<unsigned char *>(ivBytes2.data()),
                    AES_DECRYPT);

    return plainBytes.constData();
}