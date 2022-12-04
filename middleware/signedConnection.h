//
// Created by Andrii on 11/6/2022.
//

#ifndef UPA_ATM_SIGNEDCONNECTION_H
#define UPA_ATM_SIGNEDCONNECTION_H
#include "transfer.h"
#include "deposit.h"
#include "backend/bankProvider.h"
#include "connectionDetails.h"

class SignedConnection {
    ConnectionDetails _details;
    Bank &bank = BankProvider::getBank();
public:

    explicit SignedConnection(const ConnectionDetails &details);

    TransferDetails getTransferDetails(const TransferRequest &);

    void transferMoney(const TransferRequest &);


    void withdrawMoney(const WithdrawalRequest &);

    DepositDetails getDepositDetails(const DepositRequest &r);

    void depositMoney(const DepositRequest &r);

    WithdrawalDetails getWithdrawalDetails(const WithdrawalRequest &r);

    inline const Credentials &credentials() const { return _details.getCredentials(); }

    inline const CardInfo getCardInfo() {
        return bank.authorizedCall(credentials(), Bank::getCardInfo);
    }
};

class ParentConnection {
private:
    ConnectionDetails _details;
    ChildCard _childCard;
    Bank bank = BankProvider::getBank();
public:
   inline const Credentials &credentials() const { return _details.getCredentials(); }

   void limitChildMoney(uint money);
};
#endif //UPA_ATM_SIGNEDCONNECTION_H
