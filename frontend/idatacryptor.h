#ifndef IDATACRYPTOR_H
#define IDATACRYPTOR_H

#include <QByteArray>

class IDataCryptor
{
protected:
    inline virtual QByteArray encryptData(const QByteArray& data) { return data; }
    inline virtual QByteArray decryptData(const QByteArray& encryptedData) { return encryptedData; }
};

#endif // IDATACRYPTOR_H
