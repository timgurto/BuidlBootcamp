#pragma once

#include "PublicKey.h"
#include "Transaction.h"

class Bank {
 public:
  Transaction issue(Currency amount, PublicKey recipient);

  Currency checkBalance(PublicKey account) const;

  void handleTransaction(const Transaction& tx);

 private:
  std::map<PublicKey, Currency> m_balances;
  std::map<TxID, Transaction> m_transactions;

  bool inputsMatchOutputs(const Transaction& tx) const;
  /**/ Currency total(const Transaction::Inputs& inputs) const;
  /**/ static Currency total(const Transaction::Outputs& outputs);

  void clearCoinsFromInputs(const Transaction& tx);
  /**/ PublicKey currentOwnerOfInput(const TxInput& input) const;

  void distributeCoinsToOutputs(const Transaction::Outputs& outputs);
  /**/ void giveOutputToItsRecipient(const TxOutput& output);
};
