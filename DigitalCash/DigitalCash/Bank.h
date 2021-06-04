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
  /**/ bool amountsMatch(const Transaction::Inputs& inputs,
                       Transaction::Outputs& outputs) const;
  /******/ Currency sum(const Transaction::Inputs& inputs) const;
  /******/ static Currency sum(const Transaction::Outputs& outputs);
  /**/ bool inputsAreSigned(const Transaction& tx) const;

  void unclassifyAsUTXOs(const Transaction::Inputs& inputs);
  /**/ const TxOutput& correspondingUTXO(const TxInput& input) const;

  void classifyAsUTXOs(const Transaction::Outputs& outputs);

  // Helpers
  const TxOutput& lookupOutput(const TxOutputID& outputID) const;
};
