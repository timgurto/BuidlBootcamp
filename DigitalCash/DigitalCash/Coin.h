#pragma once

#include <vector>

#include "Transfer.h"

class Coin {
 public:
  static Coin CreateByIssuingTo(const PublicKey &issuee);
  static Coin CreateByDeserialising(const std::string &serialisedCoin);
  /**/ void readAndAddTransaction(std::istringstream &serialisedCoin);
  static Coin CreateEmptyForTesting() { return {}; }

  std::string serialise() const;

  bool operator==(const Coin &rhs) const;
  bool operator!=(const Coin &rhs) const { return !(*this == rhs); }

  void appendTransfer(const Transfer &txn) { m_transfers.push_back(txn); }

  bool isValid() const;
  /**/ bool coinWasIssuedByTheGovernment() const;
  /**/ bool allTransactionsHaveValidSignatures() const;
  /**/ bool eachSpenderWasTheOwner() const;

  const Transfer *getLastTransfer() const;

 private:
  std::vector<Transfer> m_transfers;

  Coin() = default;                         // CreateEmptyForTesting
  Coin(const std::string &serialisedCoin);  // CreateByDeserialising
  Coin(const PublicKey &issuee);            // CreateByIssuingTo
};
