#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H
#include <QSqlError>
#include <stdexcept>
#include <QDebug>
#include <sstream>

inline std::string makeErrorMsg(const QString& query, const QSqlError& error, const char* src);

class DatabaseException : public std::runtime_error{
public:
    DatabaseException(const std::string &msg) : std::runtime_error(msg) {
        qDebug() << "DatabaseException created with message:" << msg.c_str();
    }
};

class SqlQueryException : public DatabaseException {
public:
    SqlQueryException(const QString& query, const QSqlError& error, const char* src = "") :
        DatabaseException(makeErrorMsg(query, error, src)) {}
};

inline std::string makeErrorMsg(const QString& query, const QSqlError& error, const char* src)
{
    std::stringstream sstream;
    sstream << src << ": " <<
               "while executing query '" <<
               query.toLatin1().toStdString() <<
               "'\n\t" <<
               error.text().toLatin1().toStdString();
    return sstream.str();
}

#endif // DATABASEEXCEPTION_H
