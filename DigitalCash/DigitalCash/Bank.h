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
  std::map<TxID, Transaction> m_transactions;
  std::set<TxOutputID> m_unspentOutputs;

  void registerTransaction(const Transaction& tx);

  bool transactionIsValid(const Transaction& tx) const;
  /**/ bool inputsAreUnspent(const Transaction::Inputs& inputs) const;
  /**/ bool inputsMatchOutputs(const Transaction& tx) const;
  /******/ Currency sum(const Transaction::Inputs& inputs) const;
  /******/ static Currency sum(const Transaction::Outputs& outputs);
  /**/ bool inputsAreSigned(const Transaction& tx) const;

  void takeCoinsFromInputs(const Transaction::Inputs& inputs);
  /**/ const TxOutput& correspondingUTXO(const TxInput& input) const;

  void giveCoinsToOutputs(const Transaction::Outputs& outputs);
  /**/ void giveOutputToItsRecipient(const TxOutput& output);
};
