#ifndef DB_TRANSACTION_H
#define DB_TRANSACTION_H

#include "middleware/types.h"
#include <QVariant>
#include <QSqlRecord>
#include <QDate>
#include <QSqlDatabase>

class DBTransaction {
private:
    Optional<ullong> _id;
    Optional<ullong> _sender_id;
    Optional<ullong> _receiver_id;
    Optional<ullong> _amount;
    Optional<ullong> _fee; // replace with ullong
    Optional<QDateTime> _time;
    Optional<QString> _description;
public:
    DBTransaction() {}
    DBTransaction(ullong id,
                  ullong sender_id,
                  ullong receiver_id,
                  ullong amount,
                  ullong fee,
                  const QDateTime& time,
                  const QString& description) :
    _id(id),
    _sender_id(sender_id),
    _receiver_id(receiver_id),
    _amount(amount),
    _fee(fee),
    _time(time),
    _description(description) {}

    DBTransaction(const QSqlRecord& record) :
        _id(record.value("id").value<ullong>()),
        _sender_id(record.value("sender_id").value<ullong>()),
        _receiver_id(record.value("receiver_id").value<ullong>()),
        _amount(record.value("amount").value<ullong>()),
        _fee(record.value("fee").value<ullong>()),
        _time(record.value("time").value<QDateTime>()),
        _description(record.value("description").value<QString>()) {}

    inline void setId(ullong id) {_id = id;}
    inline void setSenderId(ullong sender_id) {_sender_id = sender_id;}
    inline void setReceiverId(ullong receiver_id) {_receiver_id = receiver_id;}
    inline void setAmount(ullong amount) {_amount = amount;}
    inline void setFee(ullong fee) {_fee = fee;}
    inline void setTime(const QDateTime& time) {_time = time;}
    inline void setDescription(const QString& description) {_description = description;}

    inline Optional<ullong> getId() const {return _id;}
    inline Optional<ullong> getSenderId() const {return _sender_id;}
    inline Optional<ullong> getReceiverId() const {return _receiver_id;}
    inline Optional<ullong> getAmount() const {return _amount;}
    inline Optional<ullong> getFee() const {return _fee;}
    inline Optional<QDateTime> getTime() const {return _time;}
    inline Optional<QString> getDescription() const {return _description;}

    static ullong create(const DBTransaction& card, const QSqlDatabase& db = QSqlDatabase::database());
    static Vector<DBTransaction> selectAll(const QSqlDatabase& db = QSqlDatabase::database());
    static DBTransaction selectById(ullong id, const QSqlDatabase& db = QSqlDatabase::database());
    static Vector<DBTransaction> selectSpendingsByPeriod(ullong card_id,
                                                         QDateTime start,
                                                         QDateTime end,
                                                         const QSqlDatabase& db = QSqlDatabase::database());
};



#endif // DB_TRANSACTION_H
