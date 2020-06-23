#include "clienthandler.h"

#include "databaseinterface.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QCoreApplication>

ClientHandler::ClientHandler(QHttpRequest *req, QHttpResponse *res)
    : QObject(req /* as parent*/)
{
    dbInterface = DatabaseInterface::instance();

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

    if (!req->url().url().compare("/courses/setmark")) {
//        enrollStudentOnCourse(req, res);
    }

    if (!req->url().url().compare("/students/getreport")) {
        //        enrollStudentOnCourse(req, res);
    }
}

ClientHandler::~ClientHandler()
{
    qDebug("  ~ClientHandler: I've being called automatically!");
}

void ClientHandler::addCourse(QHttpRequest *req, QHttpResponse *res)
{
    //    // automatically collect http body(data) upto 1KB
    req->collectData(1024);

    //    // when all the incoming data are gathered, send some response to client.
    req->onEnd([req, res]() {

        if (req->collectedData().size() > 0) {
            QString newCourseName = req->collectedData().constData();
            qDebug() << QString("adding a new course: %1").arg(newCourseName);

            int returnedID = DatabaseInterface::instance()->addCourse(newCourseName);

            if (returnedID != -1) {
                QString message = QString("new course %1 has beed added to database with ID : %2")
                                  .arg(newCourseName)
                                  .arg(returnedID);

                // return results
                res->setStatusCode(qhttp::ESTATUS_OK);
                res->addHeaderValue("content-length", message.size());
                res->addHeaderValue("Access-Control-Allow-Origin", QString("*"));
                res->end(message.toUtf8());
            } else {
                qDebug() << "database interface did not return correctly";
            }
        } else {
            res->setStatusCode(qhttp::ESTATUS_BAD_REQUEST);
            QString message = QString("course name did not received");
            res->addHeaderValue("content-length", message.size());
            res->addHeaderValue("Access-Control-Allow-Origin", QString("*"));
            res->end(message.toUtf8());
        }
    });
}

void ClientHandler::requestCourses(QHttpRequest *req, QHttpResponse *res)
{
    Q_UNUSED(req);

    QJsonArray allCoursesArray = dbInterface->requestCourses();

    if (allCoursesArray.size() > 0) {
        QString message = QJsonDocument(allCoursesArray).toJson();

        res->setStatusCode(qhttp::ESTATUS_OK);
        res->addHeaderValue("content-length", message.size());
        res->addHeaderValue("Access-Control-Allow-Origin", QString("*"));
        res->addHeaderValue("content-type", QString("application/json"));
        res->end(message.toUtf8());
    } else {
        QString message = QString("courses did not received");

        res->setStatusCode(qhttp::ESTATUS_EXPECTATION_FAILED);
        res->addHeaderValue("content-length", message.size());
        res->addHeaderValue("Access-Control-Allow-Origin", QString("*"));
        res->end(message.toUtf8());
    }
}

void ClientHandler::addStudent(QHttpRequest *req, QHttpResponse *res)
{
    //    // automatically collect http body(data) upto 1KB
    req->collectData(1024);

    //    // when all the incoming data are gathered, send some response to client.
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
                res->addHeaderValue("Access-Control-Allow-Origin", QString("*"));
                res->end(message.toUtf8());
            } else {
                qDebug() << "database interface did not return correctly";
            }
        } else {
            res->setStatusCode(qhttp::ESTATUS_BAD_REQUEST);
            QString message = QString("student name did not received");
            res->addHeaderValue("content-length", message.size());
            res->addHeaderValue("Access-Control-Allow-Origin", QString("*"));
            res->end(message.toUtf8());
        }
    });
}

void ClientHandler::requestStudents(QHttpRequest *req, QHttpResponse *res)
{
    Q_UNUSED(req);

    QJsonArray allStudenstArray = dbInterface->requestStudents();

    if (allStudenstArray.size() > 0) {
        QString message = QJsonDocument(allStudenstArray).toJson();

        res->setStatusCode(qhttp::ESTATUS_OK);
        res->addHeaderValue("content-length", message.size());
        res->addHeaderValue("Access-Control-Allow-Origin", QString("*"));
        res->addHeaderValue("content-type", QString("application/json"));
        res->end(message.toUtf8());
    } else {
        QString message = QString("courses did not received");

        res->setStatusCode(qhttp::ESTATUS_EXPECTATION_FAILED);
        res->addHeaderValue("content-length", message.size());
        res->addHeaderValue("Access-Control-Allow-Origin", QString("*"));
        res->end(message.toUtf8());
    }
}

void ClientHandler::enrolStudentOnCourse(QHttpRequest *req, QHttpResponse *res)
{
    qDebug() << "requested to enrol";

    //    // automatically collect http body(data) upto 1KB
    req->collectData(1024);

    //    // when all the incoming data are gathered, send some response to client.
    req->onEnd([req, res]() {
        if (req->collectedData().size() > 0) {
            QString receivedData = req->collectedData().constData();
            QUrlQuery formData(receivedData);

            int returnedID = DatabaseInterface::instance()->addEnrolment(
                                 formData.queryItemValue("student").toInt(),
                                 formData.queryItemValue("course").toInt());

            if (returnedID != -1) {
                QString message = QString("Enrolment has been succeeded and added to database\n"
                                          "reference ID : %1\n\n\tyou can close this window").arg(returnedID);

                // return results
                res->setStatusCode(qhttp::ESTATUS_OK);
                res->addHeaderValue("content-length", message.size());
                res->addHeaderValue("Access-Control-Allow-Origin", QString("*"));
                res->end(message.toUtf8());
            } else {
                res->setStatusCode(qhttp::ESTATUS_BAD_REQUEST);
                QString message = QString("student name did not received");
                res->addHeaderValue("content-length", message.size());
                res->addHeaderValue("Access-Control-Allow-Origin", QString("*"));
                res->end(message.toUtf8());
            }
        }
    });
}
