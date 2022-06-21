#ifndef INTFC_DATA_HANDLER
#define INTFC_DATA_HANDLER

#include <QString>
#include <QObject>

class IDataHandler : protected QObject
{
    Q_OBJECT;
    public slots: 
        virtual void handleData(QString data) = 0;
};

#endif