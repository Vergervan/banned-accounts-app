#ifndef APPNETWORKHANDLER_H
#define APPNETWORKHANDLER_H

#include "idatahandler.h"
#include <iostream>
#include <QSqlDatabase>
#include <QtSql>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include "simplecryptor.h"
#include <QRegularExpression>

class AppNetworkHandler : public IDataHandler
{
public:
        void handleData(QTcpSocket* socket, IDataHandler::Message msg) Q_DECL_OVERRIDE;
        inline void setDatabase(QSqlDatabase* db) { _dbptr = db; }
        AppNetworkHandler();
        inline AppNetworkHandler(QSqlDatabase* db) { setDatabase(db); }
        ~AppNetworkHandler();
        inline ushort getRawCode(MessageCode code){
            return static_cast<unsigned short>(code);
        }
private:
        IDataHandler::Message signHandle(MessageCode, QString, QString);
        bool addAccount(const QJsonObject& obj);
        bool updateAccount(const QJsonObject& obj);
        QSqlDatabase* _dbptr;
        SimpleCryptor* _cryptor;
};

#endif // APPNETWORKHANDLER_H
