#ifndef DB_CATEGORY_H
#define DB_CATEGORY_H

#include "middleware/types.h"
#include <QSqlRecord>
#include <QString>
#include <QSqlDatabase>
#include <QVariant>

class DBCategory {
private:
    Optional<ullong> _id;
    Optional<QString> _name;
    Optional<double> _fee_rate;
public:
    DBCategory() {}
    DBCategory(ullong id, const QString& name, double fee_rate) :
    _id(id),
    _name(name),
    _fee_rate(fee_rate)
    {}
    DBCategory(const QSqlRecord& record) :
        DBCategory(record.value("id").value<ullong>(),
            record.value("name").value<QString>(),
            record.value("fee_rate").value<double>())
    {}

    inline void setId(ullong id) {_id = id;}
    inline void setName(const QString& name) {_name = name;}
    inline void setFeeRate(double fee_rate) {_fee_rate = fee_rate;}

    inline Optional<ullong> getId() const {return _id;}
    inline Optional<QString> getName() const {return _name;}
    inline Optional<double> getFeeRate() const {return _fee_rate;}

    void to(DBCategory& other) const;

    static ullong create(const DBCategory& card);
    static Vector<DBCategory> selectAll();
    static DBCategory selectById(ullong id);
    static void update(const DBCategory& card);
    static void remove(ullong id);
};


#endif // DB_CATEGORY_H
