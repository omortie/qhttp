#include "databaseinterface.h"

#include <QtSql>
#include <QJsonDocument>

#include <QDebug>

DatabaseInterface *DatabaseInterface::dbInterface = nullptr;

DatabaseInterface::DatabaseInterface(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("../../example/teamyarserver/database/TeamyarDB.sqlite");
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
        qDebug() << "new student ID returned : " << returnedID;
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
        qDebug() << "new course ID returned : " << returnedID;
    }

    return returnedID;
}

int DatabaseInterface::addEnrolment(int studentNumber, int courseNumber)
{
    int returnedID = -1;

    QSqlQuery query;
    query.prepare("INSERT INTO T_Enrolments (ID, Student, Course) "
                  "VALUES (:id, :student, :course)");
    query.bindValue(":student", studentNumber);
    query.bindValue(":course", courseNumber);

    if (query.exec()) {
        returnedID = query.lastInsertId().toInt();
        qDebug() << "returned enrolment id : " << returnedID;
    }

    return returnedID;
}

QJsonArray DatabaseInterface::requestCourses()
{
    QJsonArray allCoursesArray;

    // requesting all courses from database
    QSqlQuery query;
    if (query.exec("SELECT * FROM T_Courses")) {
        QJsonObject courseObj;
        while (query.next()) {
            courseObj.insert("ID", query.value(0).toString());
            courseObj.insert("name", query.value(1).toString());

            allCoursesArray.append(courseObj);
        }

        qDebug() << "requesting all courses done";
    }

    return allCoursesArray;
}

QJsonArray DatabaseInterface::requestStudents()
{
    QJsonArray allStudentsArray;

    // requesting all courses from database
    QSqlQuery query;
    if (query.exec("SELECT * FROM T_Students")) {
        QJsonObject studentObj;
        while (query.next()) {
            studentObj.insert("ID", query.value(0).toString());
            studentObj.insert("name", query.value(1).toString());

            allStudentsArray.append(studentObj);
        }

        qDebug() << "requesting all students done";
    }

    return allStudentsArray;
}

QString DatabaseInterface::lastErr()
{
    return db.lastError().text();
}
