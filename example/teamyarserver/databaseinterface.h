#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QObject>

#include <QSqlDatabase>

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
    QJsonArray requestCourses();
    QJsonArray requestStudents();

    QString lastErr();

private:
    QSqlDatabase db;
};

#endif // DATABASEINTERFACE_H
