#include "clienthandler.h"

#include "databaseinterface.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

ClientHandler::ClientHandler(QHttpRequest *req, QHttpResponse *res)
    : QObject(req /* as parent*/)
{
    /* checking the path of the request url and decide what server should do
     passing request and pointer of the response to the appropriate slots in client handler
    so the response can be populated accorodingly */

    if (!req->url().url().compare("/")) {
        requestIndexPage(req, res);
    }

    if (!req->url().url().compare("/students/add")) {
        addStudent(req, res);
    }

    if (!req->url().url().compare("/students/request")) {
        requestStudents(req, res);
    }

    if (!req->url().url().compare("/courses/add")) {
        addCourse(req, res);
    }

    if (!req->url().url().compare("/courses/request")) {
        requestCourses(req, res);
    }

    if (!req->url().url().compare("/courses/enrol")) {
        enrolStudentOnCourse(req, res);
    }

    if (!req->url().url().compare("/enrolments/request")) {
        requestEnrolments(req, res);
    }

    if (!req->url().url().compare("/enrolments/setmark")) {
        setMark(req, res);
    }
}

ClientHandler::~ClientHandler()
{
    qDebug("  ~ClientHandler: I've being called automatically!");
}

void ClientHandler::requestIndexPage(QHttpRequest *req, QHttpResponse *res)
{
    // this request is for loading the main html index file located in the example folder
    // to be shown in the client web browser
    Q_UNUSED(req);

    // looking for the main html file and read the content of it to send back to the client
    QFile indexFile("../../example/teamyarserver/client/HTMLFile.html");

    if (indexFile.exists() && indexFile.open(QIODevice::ReadOnly)) {
        QString message = indexFile.readAll();

        // return results
        res->setStatusCode(qhttp::ESTATUS_OK);
        res->addHeaderValue("content-length", message.size());
        res->end(message.toUtf8());
    } else {
        // if file does not exist or the file cannot be open the server respond NOT FOUND error
        QString message = QString("NOT FOUND 404 or the web page cannot be ACCESSED!");

        res->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
        res->addHeaderValue("content-length", message.size());
        res->end(message.toUtf8());
    }
}

void ClientHandler::addCourse(QHttpRequest *req, QHttpResponse *res)
{
    // automatically collect http body(data) upto 1KB
    req->collectData(1024);

    // when all the incoming data are gathered, send some response to client.
    req->onEnd([req, res]() {
        // check if the user sends the name of the new course receive it and send it to be
        // added to database as a new course with database interface
        if (req->collectedData().size() > 0) {
            QString newCourseName = req->collectedData().constData();
            qDebug() << QString("adding a new course: %1").arg(newCourseName);

            int returnedID = DatabaseInterface::instance()->addCourse(newCourseName);

            // if the returned ID is -1 it will demonstrate that new course operation was not succeeded
            //! \todo should implement a modern way of lastError mechanism in database interface
            if (returnedID != -1) {
                QString message = QString("new course %1 has beed added to database with ID : %2")
                                  .arg(newCourseName)
                                  .arg(returnedID);

                // return results
                res->setStatusCode(qhttp::ESTATUS_OK);
                res->addHeaderValue("content-length", message.size());
                res->end(message.toUtf8());
            } else {
                QString message = QString("database interface did not return correctly , adding new course error");

                qDebug() << message;
                // return the error
                res->setStatusCode(qhttp::ESTATUS_CONFLICT);
                res->addHeaderValue("content-length", message.size());
                res->end(message.toUtf8());
            }
        } else {
            // return the error
            res->setStatusCode(qhttp::ESTATUS_BAD_REQUEST);
            QString message = QString("Error on adding new course - no data provided to server");
            res->addHeaderValue("content-length", message.size());
            res->end(message.toUtf8());
        }
    });
}

void ClientHandler::requestCourses(QHttpRequest *req, QHttpResponse *res)
{
    // client requested all of the courses in the database
    Q_UNUSED(req);

    // a simple SELECT SQL statement and return the results
    QJsonArray allCoursesArray = DatabaseInterface::instance()->requestCourses();

    QString message = QJsonDocument(allCoursesArray).toJson();

    res->setStatusCode(qhttp::ESTATUS_OK);
    res->addHeaderValue("content-length", message.size());
    res->addHeaderValue("content-type", QString("application/json"));
    res->end(message.toUtf8());
}

void ClientHandler::addStudent(QHttpRequest *req, QHttpResponse *res)
{
    // the description here is like the addCourse method just adding new student instead
    req->collectData(1024);

    req->onEnd([req, res]() {

        if (req->collectedData().size() > 0) {
            QString newStudentName = req->collectedData().constData();
            qDebug() << QString("adding a new student: %1").arg(newStudentName);

            int returnedID = DatabaseInterface::instance()->addStudent(newStudentName);

            if (returnedID != -1) {
                QString message = QString("new student %1 has beed added to database with ID : %2")
                                  .arg(newStudentName)
                                  .arg(returnedID);

                // return results
                res->setStatusCode(qhttp::ESTATUS_OK);
                res->addHeaderValue("content-length", message.size());
                res->end(message.toUtf8());
            } else {
                QString message = QString("database interface did not return correctly , adding new student error");

                qDebug() << message;
                // return the error
                res->setStatusCode(qhttp::ESTATUS_CONFLICT);
                res->addHeaderValue("content-length", message.size());
                res->end(message.toUtf8());
            }
        } else {
            res->setStatusCode(qhttp::ESTATUS_BAD_REQUEST);
            QString message = QString("student name did not received");
            res->addHeaderValue("content-length", message.size());
            res->end(message.toUtf8());
        }
    });
}

void ClientHandler::requestStudents(QHttpRequest *req, QHttpResponse *res)
{
    // client requested all of the students in the database
    Q_UNUSED(req);

    // a simple SELECT SQL statement and return the results
    QJsonArray allStudenstArray = DatabaseInterface::instance()->requestStudents();

    QString message = QJsonDocument(allStudenstArray).toJson();

    res->setStatusCode(qhttp::ESTATUS_OK);
    res->addHeaderValue("content-length", message.size());
    res->addHeaderValue("content-type", QString("application/json"));
    res->end(message.toUtf8());
}

void ClientHandler::enrolStudentOnCourse(QHttpRequest *req, QHttpResponse *res)
{
    qDebug() << "requested to enrol on a specific course";

    req->collectData(1024);

    req->onEnd([req, res]() {
        // check if the client sends the data the proceed to process the input data
        if (req->collectedData().size() > 0) {
            // process the input data of HTML form into the ID of the student and the relevant course
            QString receivedData = req->collectedData().constData();
            QUrlQuery formData(receivedData);

            int returnedID = DatabaseInterface::instance()->addEnrolment(
                                 formData.queryItemValue("student").toInt(),
                                 formData.queryItemValue("course").toInt());

            // check if the returned ID is -1 or not (to determine if the operation was successful or not
            if (returnedID != -1) {
                QString message = QString("Enrolment has been succeeded and added to database\n"
                                          "reference ID : %1\n\n\tyou can close this window").arg(returnedID);

                // return results
                res->setStatusCode(qhttp::ESTATUS_OK);
                res->addHeaderValue("content-length", message.size());
                res->end(message.toUtf8());
            } else {
                // return the error to the client
                res->setStatusCode(qhttp::ESTATUS_BAD_REQUEST);
                QString message =
                    QString("enroling student on a new course was NOT successful and error happened!");
                res->addHeaderValue("content-length", message.size());
                res->end(message.toUtf8());
            }
        }
    });
}

void ClientHandler::requestEnrolments(QHttpRequest *req, QHttpResponse *res)
{
    qDebug() << "requested all enrolments of a specific student";

    req->collectData(1024);

    req->onEnd([req, res]() {
        if (req->collectedData().size() > 0) {
            QString studentID = req->collectedData().constData();
            qDebug() << "requested all enrolments of the student ID " << studentID;

            QJsonArray allEnrolments = DatabaseInterface::instance()->requestEnrolments(studentID.toInt());

            QString message = QJsonDocument(allEnrolments).toJson();

            res->setStatusCode(qhttp::ESTATUS_OK);
            res->addHeaderValue("content-length", message.size());
            res->addHeaderValue("content-type", QString("application/json"));
            res->end(message.toUtf8());
        }
    });
}

void ClientHandler::setMark(QHttpRequest *req, QHttpResponse *res)
{
    qDebug() << "setting mark for a specific student";

    req->collectData(1024);

    req->onEnd([req, res] {
        if (req->collectedData().size() > 0)
        {
            // receive the enrolment ID and the appropriate mark by a simple protocol (separated with semicolon)
            // and parse them for database interface
            QString receivedData = req->collectedData().constData();

            QStringList parsedData = receivedData.split(";");

            int returnedID = DatabaseInterface::instance()->setMark(parsedData.at(0).toInt()
                                                                    , parsedData.at(1).toDouble());

            qDebug() << "Setting mark for enrolment ID " << parsedData.at(0).toInt() << " and mark : " <<
                     parsedData.at(1).toDouble();

            // check the returned ID as the result of the operation and send success or error to the client
            if (returnedID != -1) {
                QString message = QString("Mark has been set and added to database\n"
                                          "reference ID : %1\n\n\tyou can close this window").arg(returnedID);

                // return results
                res->setStatusCode(qhttp::ESTATUS_OK);
                res->addHeaderValue("content-length", message.size());
                res->end(message.toUtf8());
            } else {
                res->setStatusCode(qhttp::ESTATUS_BAD_REQUEST);
                QString message = QString("student name did not received");
                res->addHeaderValue("content-length", message.size());
                res->end(message.toUtf8());
            }
        }
    });
}
