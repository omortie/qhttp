#include "databaseinterface.h"

#include <QtSql>

#include <QDebug>

DatabaseInterface *DatabaseInterface::dbInterface = nullptr;

DatabaseInterface::DatabaseInterface(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("../../example/teamyarserver/TeamyarDB.sqlite");
    QFile dbFile(db.databaseName());
    if (!dbFile.exists()) {
        qDebug() << db.databaseName() + " Database doesn't exists";
    } else if (!db.open()) {
        qDebug() << db.databaseName() + " Database Cannot be Opened!!!\n" + db.lastError().text();
    } else {
        qDebug() << "list of tables loaded: " << db.tables();
    }
}

DatabaseInterface *DatabaseInterface::instance()
{
    if (!dbInterface) {
        dbInterface = new DatabaseInterface();
    }
    return dbInterface;
}

DatabaseInterface::~DatabaseInterface()
{

}

int DatabaseInterface::addStudent(QString name)
{
    int returnedID = -1;

    QSqlQuery query;
    query.prepare("INSERT INTO T_Students (ID, name) "
                  "VALUES (:id, :name)");
    query.bindValue(":name", name);

    if (query.exec()) {
        returnedID = query.lastInsertId().toInt();
        qDebug() << "returned id : " << returnedID;
    }

    return returnedID;
}

int DatabaseInterface::addCourse(QString name)
{
    int returnedID = -1;

    QSqlQuery query;
    query.prepare("INSERT INTO T_Courses (ID, name) "
                  "VALUES (:id, :name)");
    query.bindValue(":name", name);

    if (query.exec()) {
        returnedID = query.lastInsertId().toInt();
        qDebug() << "returned id : " << returnedID;
    }

    return returnedID;
}
