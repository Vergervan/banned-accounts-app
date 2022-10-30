#ifndef APPNETWORKHANDLER_H
#define APPNETWORKHANDLER_H

#include "idatahandler.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>

class AppNetworkHandler : public IDataHandler
{
public:
        void handleData(qintptr socketDscr, QString msg) override;
        void setDatabase(QSqlDatabase* db) { dbptr = db; }
        inline AppNetworkHandler(){}
        inline AppNetworkHandler(QSqlDatabase* db) { setDatabase(db); }
private:
        QSqlDatabase* dbptr;
};

#endif // APPNETWORKHANDLER_H
