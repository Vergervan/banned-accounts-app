#ifndef IMESSAGEQUEUE_H
#define IMESSAGEQUEUE_H

#include <QTcpSocket>
#include "idatahandler.h"

class IMessageQueue{
public:
    virtual void enqueueMessage(QTcpSocket*, IDataHandler::Message) = 0;
};

#endif // IMESSAGEQUEUE_H
