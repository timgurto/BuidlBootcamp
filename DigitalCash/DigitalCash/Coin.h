#pragma once

#include <vector>

#include "Transaction.h"

class Coin {
 public:
  Coin() = default;

  static Coin Deserialise(const std::string &serialisedCoin);
  std::string serialise() const;

  bool operator==(const Coin &rhs) const;
  /**/ static bool numberOfTransactionsMatch(const Coin &lhs, const Coin &rhs);
  /**/ static bool transactionsMatch(const Coin &lhs, const Coin &rhs);
  bool operator!=(const Coin &rhs) const { return !(*this == rhs); }

  void addTxn(const Transaction &txn) { m_transactions.push_back(txn); }

  bool isValid() const;
  /**/ bool coinWasIssuedByTheGovernment() const;
  /**/ bool allTransactionsHaveValidSignatures() const;

 private:
  std::vector<Transaction> m_transactions;

  Coin(const std::string &serialisedCoin);
};
