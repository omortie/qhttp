#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>

#include "qhttpserver.hpp"
#include "qhttpserverrequest.hpp"
#include "qhttpserverresponse.hpp"

class DatabaseInterface;

using namespace qhttp::server;

/** connection class for gathering incoming chunks of data from HTTP client.
 * @warning please note that the incoming request instance is the parent of
 * this QObject instance. Thus this class will be deleted automatically.
 * */
class ClientHandler : public QObject
{
public:
    explicit ClientHandler(QHttpRequest *req, QHttpResponse *res);

    virtual ~ClientHandler();

private slots:
    void addCourse(QHttpRequest *req, QHttpResponse *res);
    void addStudent(QHttpRequest *req, QHttpResponse *res);
    void enrollStudentOnCourse(QHttpRequest *req, QHttpResponse *res);

private:
    DatabaseInterface *dbInterface;
};


#endif // CLIENTHANDLER_H
