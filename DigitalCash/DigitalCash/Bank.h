#pragma once

#include "PublicKey.h"
#include "Transaction.h"

class Bank {
 public:
  Transaction issue(Currency amount, PublicKey recipient);

  Currency checkBalance(PublicKey account);

  void handleTransaction(const Transaction& tx);

 private:
  std::map<PublicKey, Currency> m_balances;
  std::map<TxID, Transaction> m_transactions;

  void distributeOutput(const TxOutput& output);
  PublicKey getTransactionSender(const Transaction& tx, Index whichInput);
};
