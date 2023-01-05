#include "../headers/simplecryptor.h"

QByteArray SimpleCryptor::encryptData(const QByteArray &data){
    SimpleCrypt crypt(m_ckey);
    return crypt.encryptToByteArray(data);
}

QByteArray SimpleCryptor::decryptData(const QByteArray &encryptedData){
    SimpleCrypt crypt(m_ckey);
    return crypt.decryptToByteArray(encryptedData);
}

void SimpleCryptor::setKey(quint64 key){
    m_ckey = key;
}
