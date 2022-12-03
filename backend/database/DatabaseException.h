#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H
#include <QSqlError>
#include <stdexcept>

inline const char* makeErrorMsg(const QString& query, const QSqlError& error, const char* src);

class DatabaseException : public std::runtime_error{
public:
    DatabaseException() : std::runtime_error("") {};
    DatabaseException(const char* msg) : std::runtime_error(msg) {}
};

class SqlQueryException : public DatabaseException {
public:
    SqlQueryException(const QString& query, const QSqlError& error, const char* src = "") :
        DatabaseException(makeErrorMsg(query, error, src)) {}
};

// todo replace with string buffer
inline const char* makeErrorMsg(const QString& query, const QSqlError& error, const char* src)
{
    return (QString(src) +
            QString(": ") +
            QString("while executing query '") +
            query +
            QString("'\n\t") +
            error.text()).toLatin1();
}

#endif // DATABASEEXCEPTION_H
