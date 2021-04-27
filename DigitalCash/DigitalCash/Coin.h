#pragma once

#include <vector>

#include "Transaction.h"

class Coin {
 public:
  static Coin CreateByIssuingTo(const PublicKey &issuee);
  static Coin CreateByDeserialising(const std::string &serialisedCoin);
  /**/ void readAndAddTransaction(std::istringstream &serialisedCoin);
  static Coin CreateEmptyForTesting() { return {}; }

  std::string serialise() const;

  bool operator==(const Coin &rhs) const;
  bool operator!=(const Coin &rhs) const { return !(*this == rhs); }

  void addTransaction(const Transaction &txn) { m_transactions.push_back(txn); }

  bool isValid() const;
  /**/ bool coinWasIssuedByTheGovernment() const;
  /**/ bool allTransactionsHaveValidSignatures() const;
  /**/ bool eachSpenderWasTheOwner() const;

 private:
  std::vector<Transaction> m_transactions;

  Coin() = default;                         // CreateEmptyForTesting
  Coin(const std::string &serialisedCoin);  // CreateByDeserialising
  Coin(const PublicKey &issuee);            // CreateByIssuingTo
};
