#include "authorizationmanager.h"

#define LOCAL_IP "127.0.0.1"
#define SERVER_IP "193.109.79.66"
#define CURRENT_IP SERVER_IP

void AuthorizationManager::quickAuth(QString username, QString pass_hash)
{
    _currentUser = username;
    _currentPasshash = pass_hash;
    initSocketConnection();
    if(_socket->state() == QAbstractSocket::ConnectedState){
        sendMessage(3, QString("{\"username\":\"%1\",\"pass\":\"%2\"}").arg(username, pass_hash));
    }
}

void AuthorizationManager::saveCurrentUser()
{
    emit sendConfigRememberUser(_currentUser, _currentPasshash);
}

void AuthorizationManager::auth(QString nick, QString pass, bool reg)
{
    _currentUser = nick;
    QCryptographicHash crypto(QCryptographicHash::Sha256);
    crypto.addData(pass.toUtf8());
    _currentPasshash = QString(crypto.result());
    if(_socket != nullptr)
    {
        if(!_socket->state() == QAbstractSocket::ConnectedState)
            _socket->connectToHost(CURRENT_IP, 25000);
        sendMessage(reg ? 6 : 3, QString("{\"username\":\"%1\",\"pass\":\"%2\"}").arg(nick, _currentPasshash));
        return;
    }

    initSocketConnection();
    if(_socket->state() == QAbstractSocket::ConnectedState)
        sendMessage(reg ? 6 : 3, QString("{\"username\":\"%1\",\"pass\":\"%2\"}").arg(nick, _currentPasshash));
}

void AuthorizationManager::initSocketConnection()
{
    _socket = new QTcpSocket;
//    _thread = new QThread;
//    connect(_thread, &QThread::started, this, [this]{
//        read();
//    });
    connect(this, SIGNAL(finishedRead(AuthorizationManager::Message)), this, SLOT(handleData(AuthorizationManager::Message)));
    connect(_socket, SIGNAL(readyRead()), this, SLOT(read()));
    connect(_socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError err){
        emit sendAuthResult(false, _socket->errorString());
        if(err == QAbstractSocket::RemoteHostClosedError){
            emit sendUserExit();
        }
    });
    //connect(_socket, &QTcpSocket::connected, [=, &crypto]() { sendMessage(reg ? 6 : 3, QString("{\"username\":\"%1\",\"pass\":\"%2\"}").arg(nick, QString(crypto.result()))); });
    _socket->connectToHost(CURRENT_IP, 25000);
    _socket->waitForConnected(1000);
    _stream = new QDataStream(_socket);
}

void AuthorizationManager::sendMessage(int code, QString mes)
{
    //auto cryptedData = _cryptor->encryptData(mes);
    ushort shortCode = static_cast<ushort>(code);
    QDataStream stream(this->_socket);
    stream << shortCode;
    _socket->write(mes.toStdString().c_str());
}

void AuthorizationManager::read()
{
    QByteArray total_data, data_buffer;
    Message msg;
    ushort code;
    *(_stream) >> code;
    msg.code = static_cast<MessageCode>(code);
    while(true)
    {
        data_buffer = _socket->read(1024);
        if(data_buffer.isEmpty()) {
            break;
        }
        total_data.append(data_buffer);
    }
    msg.data = QString(total_data);
    emit finishedRead(msg);
}

void AuthorizationManager::handleData(AuthorizationManager::Message msg)
{
    if(!msg.data.toStdString().empty()){
        qDebug() << msg.data;
    }
    switch(msg.code)
    {
        case AcceptSignIn:
            parseAccounts(msg.data);
        case AcceptSignUp:
            emit sendAuthResult(true, _currentUser);
        break;
        case ErrorSignIn:
        case ErrorSignUp:
            _currentUser.clear();
            emit sendAuthResult(false, msg.data);
        break;
        case Ping:
        case MonoPing:
            sendMessage(11);
        break;
        default: break;
    }
}

void AuthorizationManager::sendDisconnect(){
    if(_socket != nullptr){
        QDataStream stream(_socket);
        stream << (static_cast<ushort>(Disconnect));
        qDebug() << "Send disconnect";
        _socket->disconnectFromHost();
        _socket->abort();
        _socket->close();
    }
}

void AuthorizationManager::parseAccounts(const QString& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    auto accArray = doc.object().value("accounts").toArray();
    foreach (const QJsonValue& val, accArray){
        auto obj = val.toObject();
        QString numStr = obj["days_left"].toString();
        QString time = "00 00:00";
        double num = numStr.toDouble();
        if(num > 0){
            int days = (int)num;
            num -= days;
            num *= 24;
            int hours = (int)num;
            num -= hours;
            num *= 60;
            int minutes = (int)num;
            time = QString("%1,%2,%3").arg(days).arg(hours).arg(minutes);
        }
        emit sendAccountData(obj["nick"].toString(), obj["login"].toString(), time);
    }
}

AuthorizationManager::AuthorizationManager(){}
AuthorizationManager::~AuthorizationManager(){
    if(_socket != nullptr)
    {
        _socket->close();
        delete _socket;
        delete _stream;
    }
}
