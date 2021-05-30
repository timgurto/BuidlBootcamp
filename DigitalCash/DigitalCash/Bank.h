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

  void registerTransaction(const Transaction& tx);

  bool inputsMatchOutputs(const Transaction& tx) const;
  /**/ Currency total(const Transaction::Inputs& inputs) const;
  /**/ static Currency total(const Transaction::Outputs& outputs);
  bool inputsAreSigned(const Transaction& tx) const;

  void removeCoinsFromInputs(const Transaction& tx);
  /**/ const TxOutput& correspondingUTXO(const TxInput& input) const;

  void addCoinsToOutputs(const Transaction::Outputs& outputs);
  /**/ void giveOutputToItsRecipient(const TxOutput& output);
};
