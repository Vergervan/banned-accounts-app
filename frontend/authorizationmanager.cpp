#include "authorizationmanager.h"

void AuthorizationManager::auth(QString nick, QString pass, bool reg)
{
    _currentUser = nick;
    QCryptographicHash crypto(QCryptographicHash::Sha256);
    crypto.addData(pass.toUtf8());
    if(_socket != nullptr)
    {
        sendMessage(reg ? 6 : 3, QString("{\"username\":\"%1\",\"pass\":\"%2\"}").arg(nick, QString(crypto.result())));
        return;
    }

    _socket = new QTcpSocket;
//    _thread = new QThread;
//    connect(_thread, &QThread::started, this, [this]{
//        read();
//    });
    connect(this, SIGNAL(finishedRead(AuthorizationManager::Message)), this, SLOT(handleData(AuthorizationManager::Message)));
    connect(_socket, SIGNAL(readyRead()), this, SLOT(read()));
    connect(_socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError){ emit sendAuthResult(false, _socket->errorString()); delete _socket; _socket = nullptr; });

    _socket->connectToHost("127.0.0.1", 25000);
    _stream = new QDataStream(_socket);
    sendMessage(reg ? 6 : 3, QString("{\"username\":\"%1\",\"pass\":\"%2\"}").arg(nick, QString(crypto.result())));
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
        case AcceptSignUp:
            emit sendAuthResult(true, "");
        break;
        case ErrorSignIn:
        case ErrorSignUp:
            _currentUser.clear();
            emit sendAuthResult(false, msg.data);
        break;
        case AddAccount:

        break;
        case ErrorAccount:

        break;
        case UserAccountsData:

        break;
        case Ping:
        case MonoPing:
            sendMessage(11);
        break;
    }
}

void AuthorizationManager::sendDisconnect(){
    qDebug() << "Send disconnect";
    if(_socket != nullptr){
        _socket->disconnectFromHost();
        _socket->abort();
        _socket->close();
    }
}

AuthorizationManager::AuthorizationManager()
{
    _cryptor = new SimpleCryptor(0x04534FAB057D);
}

AuthorizationManager::~AuthorizationManager(){
    if(_socket != nullptr)
    {
        _socket->close();
        delete _socket;
        delete _stream;
    }
    delete _cryptor;
}
