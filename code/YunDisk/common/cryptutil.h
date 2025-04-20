#ifndef CRYPTUTIL_H
#define CRYPTUTIL_H

#include <QObject>

class CryptUtil
{
public:
    static QString md5Text(const QString &plaintext);
    static QByteArray md5Bytes(const QByteArray &plainBytes);

    static QString sha256Text(const QString &plaintext);
    static QByteArray sha256Bytes(const QByteArray &plainBytes);

    static QString desEncryptText(const QString &plaintext, const QString &key = DEFAULT_DES_KEY, const QString &mode = "ECB");
    static QString desDecryptText(const QString &ciphertext, const QString &key = DEFAULT_DES_KEY, const QString &mode = "ECB");

    static QByteArray desEncryptBytes(const QByteArray &plainBytes, const QByteArray &keyBytes = DEFAULT_DES_KEY.toUtf8(), const QString &mode = "ECB");
    static QByteArray desDecryptBytes(const QByteArray &cipherBytes, const QByteArray &keyBytes = DEFAULT_DES_KEY.toUtf8(), const QString &mode = "ECB");

    static QString aesEncryptText(const QString &plaintext, const QString &key = DEFAULT_AES_KEY, const QString &iv = DEFAULT_AES_IV);
    static QString aesDecryptText(const QString &ciphertext, const QString &key = DEFAULT_AES_KEY, const QString &iv = DEFAULT_AES_IV);

    static QByteArray aesEncryptBytes(const QByteArray &plainBytes, const QByteArray &keyBytes = DEFAULT_AES_KEY.toUtf8(), const QByteArray &ivBytes = DEFAULT_AES_IV.toUtf8());
    static QByteArray aesDecryptBytes(const QByteArray &cipherBytes, const QByteArray &keyBytes = DEFAULT_AES_KEY.toUtf8(), const QByteArray &ivBytes = DEFAULT_AES_IV.toUtf8());

public:
    static QString DEFAULT_DES_KEY;
    static QString DEFAULT_AES_KEY;
    static QString DEFAULT_AES_IV;
};

#endif // CRYPTUTIL_H