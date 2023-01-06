#ifndef AUTHORIZATIONMANAGER_H
#define AUTHORIZATIONMANAGER_H


#ifdef Q_OS_LINUX
        #include <QtQml/QtQml>
#endif
#ifdef Q_OS_WINDOWS
        #include <QtQmlCore/QtQmlCore>
#endif
#include <QCryptographicHash>
#include <QTcpSocket>
#include <QThread>
#include "simplecryptor.h"
#include <QQueue>
#include <QDebug>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class AuthorizationManager : public QObject
{
    Q_OBJECT
public:
    enum MessageCode{
        Connect = 0,
        AcceptConnect = 1,
        AlreadyConnected = 2,
        SignIn = 3,
        AcceptSignIn = 4,
        ErrorSignIn = 5,
        SignUp = 6,
        AcceptSignUp = 7,
        ErrorSignUp = 8,
        Debug = 9,
        Ping = 10,
        Pong = 11,
        AddAccount = 12,
        DeleteAccount = 13,
        ChangeAccount = 14,
        UpdateData = 15,
        SyncData = 16,
        UserAccountsData = 17,
        ErrorAccount = 18,
        Disconnect = 19,
        MonoPing = 80
    };
    struct Message{
        MessageCode code;
        QString data;
    };
    AuthorizationManager();
    ~AuthorizationManager();
public slots:
    void auth(QString nick, QString pass, bool reg);
    void sendMessage(int code, QString msg);
    inline void sendMessage(int code) { sendMessage(code, ""); }
    void sendDisconnect();
private slots:
    void read();
    void handleData(AuthorizationManager::Message msg);
signals:
    void sendAuthResult(QVariant res, QVariant err);
    void finishedRead(AuthorizationManager::Message msg);
    void sendAccountData(QVariant nick, QVariant login, QVariant time);
    void sendUserExit();
private:
    void parseAccounts(const QString& obj);
    QTcpSocket* _socket = nullptr;
    QDataStream* _stream = nullptr;
    QString _currentUser;
    QThread* _thread = nullptr;
    SimpleCryptor* _cryptor = nullptr;
    bool blockedOperation = false;
    QQueue<Message> messageQueue;
};

#endif // AUTHORIZATIONMANAGER_H
