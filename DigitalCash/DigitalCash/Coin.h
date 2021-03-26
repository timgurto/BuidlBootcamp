#pragma once

#include <vector>

#include "Transaction.h"

class Coin {
 public:
  void addTxn(const Transaction &txn) { m_transactions.push_back(txn); }
  const Transaction &getFirstTxn() const { return m_transactions.front(); }

  bool isValid() const;

 private:
  std::vector<Transaction> m_transactions;
};
