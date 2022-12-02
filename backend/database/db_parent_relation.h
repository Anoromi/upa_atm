#ifndef DB_PARENT_RELATION_H
#define DB_PARENT_RELATION_H

#include "middleware/types.h"
#include <QVariant>
#include <QSqlRecord>
#include <QSqlDatabase>

class DBParentRelation {
private:
    Optional<ullong> _parent_card_id;
    Optional<ullong> _child_card_id;
    Optional<ullong> _day_limit;
public:
    DBParentRelation() {}
    DBParentRelation(ullong parent_card_id,
                   ullong child_card_id,
                   ullong day_limit) :
    _parent_card_id(parent_card_id),
    _child_card_id(child_card_id),
    _day_limit(day_limit) {}
    DBParentRelation(const QSqlRecord& record) :
        _parent_card_id(record.value("parent_card_id").value<ullong>()),
        _child_card_id(record.value("child_card_id").value<ullong>()),
        _day_limit(record.value("day_limit").value<ullong>()) {}

    inline void setParentCardId(ullong parent_card_id) { _parent_card_id = parent_card_id; }
    inline void setChildCardId(ullong child_card_id) { _child_card_id = child_card_id; }
    inline void setDayLimit(ullong day_limit) { _day_limit = day_limit; }

    inline Optional<ullong> getParentCardId() const { return _parent_card_id; }
    inline Optional<ullong> getChildCardId() const { return _child_card_id; }
    inline Optional<ullong> getDayLimit() const { return _day_limit; }

    void to(DBParentRelation& other) const;

    static ullong create(const DBParentRelation& card);
    static Vector<DBParentRelation> selectAll();
    static Vector<DBParentRelation> selectByParentId(ullong id);
    static DBParentRelation selectByChildId(ullong id);
    static void update(const DBParentRelation& card);
    static void remove(ullong id);
};


#endif // DB_PARENT_RELATION_H
