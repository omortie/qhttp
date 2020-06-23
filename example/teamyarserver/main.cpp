#include "qhttpserver.hpp"
#include "qhttpserverconnection.hpp"
#include "qhttpserverrequest.hpp"
#include "qhttpserverresponse.hpp"

#include <QtCore/QCoreApplication>
#include <QDateTime>

#include "../include/unixcatcher.hpp"

#include "clienthandler.h"
#include "databaseinterface.h"

using namespace qhttp::server;

void listenHandler(QHttpRequest *req, QHttpResponse *res)
{
    new ClientHandler(req, res);
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
#if defined(Q_OS_UNIX)
    catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});
#endif

    QString portOrUnixSocket("10022"); // default: TCP port 10022
    if ( argc > 1 )
        portOrUnixSocket = argv[1];

    QHttpServer server(&app);
    server.listen(portOrUnixSocket, listenHandler);

    if ( !server.isListening() ) {
        fprintf(stderr, "can not listen on %s!\n", qPrintable(portOrUnixSocket));
        return -1;
    } else {
        DatabaseInterface::instance();
    }

    app.exec();
}
