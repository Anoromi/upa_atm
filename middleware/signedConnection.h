//
// Created by Andrii on 11/6/2022.
//

#ifndef UPA_ATM_SIGNEDCONNECTION_H
#define UPA_ATM_SIGNEDCONNECTION_H

#include "transfer.h"
#include "deposit.h"
#include "backend/bankProvider.h"
#include "connectionDetails.h"


class ParentConnection;

class SignedConnection {
    ConnectionDetails _details;
    Bank &bank = BankProvider::getBank();
public:

    explicit SignedConnection(const ConnectionDetails &details);

    TransferDetails getTransferDetails(const TransferRequest &);

    void transferMoney(const TransferRequest &);

    WithdrawalDetails getWithdrawalDetails(const WithdrawalRequest &r);

    void withdrawMoney(const WithdrawalRequest &);

    DepositDetails getDepositDetails(const DepositRequest &r);

    void depositMoney(const DepositRequest &r);

    Vector<Transaction> getTransactions();

    void topUpMoney(const TopUpRequest &r);

    inline const Credentials &credentials() const { return _details.getCredentials(); }

    inline const CardInfo getCardInfo() {
        return bank.authorizedCall(credentials(), Bank::getCardInfo);
    }

    Vector<ChildCard> getChildren();

    ParentConnection createParentConnection(const ChildCard &child);
};

class ParentConnection {
private:
    ConnectionDetails _details;
    ChildCard _childCard;
    Bank bank = BankProvider::getBank();
public:

    ParentConnection(const ConnectionDetails &details, const ChildCard &childCard);

    inline const Credentials &credentials() const { return _details.getCredentials(); }

    void limitChildMoney(uint money);
};

#endif //UPA_ATM_SIGNEDCONNECTION_H
