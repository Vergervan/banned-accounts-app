#ifndef AUTHORIZATIONMANAGER_H
#define AUTHORIZATIONMANAGER_H

#include <QtQmlCore/QtQmlCore>
#include <QCryptographicHash>
#include <QtSql>

class AuthorizationManager : public QObject
{
    Q_OBJECT
public:
    AuthorizationManager();
public slots:
    void auth(QString nick, QString pass, bool reg);
signals:
    void sendAuthResult(QVariant res);
private:
    QSqlDatabase db;
};

#endif // AUTHORIZATIONMANAGER_H
