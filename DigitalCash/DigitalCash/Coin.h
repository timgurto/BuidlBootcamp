#pragma once

#include <vector>

#include "Transaction.h"

class Coin {
 public:
  bool operator==(const Coin &rhs) const;
  bool operator!=(const Coin &rhs) const { return !(*this == rhs); }

  void addTxn(const Transaction &txn) { m_transactions.push_back(txn); }

  bool isValid() const;
  bool coinWasIssuedByTheGovernment() const;
  bool allTransactionsHaveValidSignatures() const;

 private:
  std::vector<Transaction> m_transactions;
};
