#ifndef DB_HOLDER_H
#define DB_HOLDER_H

#include "middleware/types.h"
#include <QVariant>
#include <QSqlRecord>
#include <QString>
#include <QSqlDatabase>

class DBHolder {
private:
    Optional<ullong> _id;
    Optional<QString> _name;
    Optional<QString> _surname;
    Optional<QString> _phone_number;
public:
    DBHolder() {}
    DBHolder(ullong id,
           const QString& name,
           const QString& surname,
           const QString& phone_number):
        _id(id),
        _name(name),
        _surname(surname),
        _phone_number(phone_number) {}
    DBHolder(const QSqlRecord& record) :
        _id(record.value("id").value<ullong>()),
        _name(record.value("name").value<QString>()),
        _surname(record.value("surname").value<QString>()),
        _phone_number(record.value("phone_number").value<QString>()) {}

    inline void setId(ullong id) {_id = id;}
    inline void setName(const QString& name) {_name = name;}
    inline void setSurname(const QString& surname) {_surname = surname;}
    inline void setPhoneNumber(const QString& phone_number) {_phone_number = phone_number;}

    inline Optional<ullong> getId() const {return _id;}
    inline Optional<QString> getName() const {return _name;}
    inline Optional<QString> getSurname() const {return _surname;}
    inline Optional<QString> getPhoneNumber() const {return _phone_number;}

    void to(DBHolder& other) const;

    static ullong create(const DBHolder& card);
    static Vector<DBHolder> selectAll();
    static DBHolder selectById(ullong id);
    static void update(const DBHolder& card);
    static void remove(ullong id);
};



#endif // DB_HOLDER_H
