#pragma once

#include <vector>

#include "Transaction.h"

class Coin {
 public:
  void addTxn(const Transaction &txn) { m_transactions.push_back(txn); }
  const Transaction &getFirstTxn() const { return m_transactions.front(); }

  bool isValid() const {
    if (m_transactions.front().sender != "Government") return false;
    for (const auto &txn : m_transactions)
      if (!txn.isSignatureValid()) return false;
    return true;
  }

 private:
  std::vector<Transaction> m_transactions;
};
