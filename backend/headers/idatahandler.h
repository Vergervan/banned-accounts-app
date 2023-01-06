#ifndef INTFC_DATA_HANDLER
#define INTFC_DATA_HANDLER

#include <QString>
#include <QObject>
#include <QSqlDatabase>
#include <QTcpSocket>

class IDataHandler : public QObject
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
    protected:
        QSqlDatabase* m_db = nullptr;
        inline void setDatabase(QSqlDatabase* db) { m_db = db; }
    public slots: 
        virtual void handleData(QTcpSocket* socket, IDataHandler::Message msg) = 0;
    signals:
            void sendPong(QTcpSocket* socket);
            void sendRemoveClient(qintptr);
};

#endif
