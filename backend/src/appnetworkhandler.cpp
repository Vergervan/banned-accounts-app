#include "../headers/appnetworkhandler.h"

void AppNetworkHandler::handleData(qintptr socketDscr, QString msg)
{
    QTcpSocket clientSocket;
    clientSocket.setSocketDescriptor(socketDscr);


    QJsonDocument doc;
    doc.fromJson(QByteArray::fromStdString(msg.toStdString()));
}
