#ifndef API_HANDLER
#define API_HANDLER

#include "idatahandler.h"

class ApiHandler : public IDataHandler
{
        void handleData(qintptr socketDscr, QString msg) override;
};

#endif
