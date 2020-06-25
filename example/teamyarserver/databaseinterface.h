#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QObject>

#include <QSqlDatabase>

//! this class will play the role of an interface between the application and the database (SQLITE in this project)
//! it will receive parameters for any operation with database and will execute SQL statements on the database driver
//! then it will return the appropriate results to the caller
//! this class is using a Singleton design pattern because all of the database transactions of the application
//! should be passed from this class as the only bridge
class DatabaseInterface : public QObject
{
    Q_OBJECT
protected:
    explicit DatabaseInterface(QObject *parent = nullptr);

    // singleton instance of database interface
    static DatabaseInterface *dbInterface;

public:
    DatabaseInterface(DatabaseInterface &other) = delete;
    void operator=(const DatabaseInterface &) = delete;

    static DatabaseInterface *instance();

    ~DatabaseInterface() override;

    int addStudent(QString name);
    int addCourse(QString name);
    int addEnrolment(int studentNumber, int courseNumber);
    int setMark(int enrolmnetID, double mark);
    QJsonArray requestCourses();
    QJsonArray requestStudents();
    QJsonArray requestEnrolments(int studentID);

    QString lastErr();

private:
    // holding database connection information
    QSqlDatabase db;
};

#endif // DATABASEINTERFACE_H
