#include "../headers/apihandler.h"
#include <QDebug>

void ApiHandler::handleData(qintptr socketDscr, QString msg)
{
    qDebug() << msg;
}
