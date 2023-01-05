#ifndef SIMPLECRYPTOR_H
#define SIMPLECRYPTOR_H

#include "idatacryptor.h"
#include "qsimplecrypt.h"

class SimpleCryptor : public IDataCryptor{
private:
    quint64 m_ckey;
public:
    inline SimpleCryptor() {}
    inline SimpleCryptor(quint64 key) { m_ckey = key; }
    QByteArray encryptData(const QByteArray &data) override;
    QByteArray encryptData(const QString &data);
    QByteArray decryptData(const QByteArray &encryptedData) override;
    void setKey(quint64 key);
};

#endif // SIMPLECRYPTOR_H
