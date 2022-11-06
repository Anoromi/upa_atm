// by Oleh Klepatskyi on 06.11.22
// todo add date field
#ifndef BOOBLEEQUE_ATM_TRANSACTION_H
#define BOOBLEEQUE_ATM_TRANSACTION_H

#include "types.h"
#include "credentials.h"
#include "tariff.h"

class Transaction {
private:
    Unique<Card> _recipientCard;
    Unique<Card> _senderCard;
    uint _money;
    Unique<Tariff> _tariff;
    // Date ??
    Unique<String> _description;
public:
	Transaction(const Card * const rc,
				const Card * const sc,
				const uint money,
				const Tariff * const t,
				const String * const d)
	: _recipientCard(rc == nullptr ? nullptr : std::make_unique<Card>(*rc)),
	  _senderCard(sc == nullptr ? nullptr : std::make_unique<Card>(*sc)),
	  _money(money),
	  _tariff(t == nullptr ? nullptr : std::make_unique<Tariff>(*t),
	  // date
	  _description(d == nullptr ? nullptr : std::make_unique<String>(d))
	{}
	
	~Transaction() {}
	
	inline Card* getRecipient() { return _recipientCard.get(); }
	inline Card* getSender() { return _senderCard.get(); }
	inline uint  getMoney() { return _money; }
	inline Tariff* getTariff() { return _tariff.get(); }
	// inline Date* getDate() {...}
	inline String* getDescription() { return desc.get(); }
};



#endif // BOOBLEEQUE_ATM_TRANSACTION_H