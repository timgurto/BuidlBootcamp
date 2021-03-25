#pragma once

#include "Transaction.h"

class Coin {
 public:
  void addTxn(const Transaction &txn) { m_firstTxn = txn; }
  const Transaction &getFirstTxn() const { return m_firstTxn; }

  bool isValid() const { return m_firstTxn.sender == "Government"; }

 private:
  Transaction m_firstTxn;
};
