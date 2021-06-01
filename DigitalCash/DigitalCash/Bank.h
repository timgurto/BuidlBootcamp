#pragma once

#include <set>
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
  std::set<std::pair<TxID, Index> > m_spentOutputs;

  void registerTransaction(const Transaction& tx);

  bool transactionIsValid(const Transaction& tx) const;
  /**/ bool inputsMatchOutputs(const Transaction& tx) const;
  /******/ Currency total(const Transaction::Inputs& inputs) const;
  /******/ static Currency total(const Transaction::Outputs& outputs);
  /**/ bool inputsAreSigned(const Transaction::Inputs& inputs) const;

  void takeCoinsFromInputs(const Transaction::Inputs& inputs);
  /**/ const TxOutput& correspondingUTXO(const TxInput& input) const;

  void giveCoinsToOutputs(const Transaction::Outputs& outputs);
  /**/ void giveOutputToItsRecipient(const TxOutput& output);
};
