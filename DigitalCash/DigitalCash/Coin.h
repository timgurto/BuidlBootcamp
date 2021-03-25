#pragma once

#include "Transaction.h"

class Coin {
 public:
  void addTxn(const Transaction &txn) { m_firstTxn = txn; }
  const Transaction &getFirstTxn() const { return m_firstTxn; }

 private:
  Transaction m_firstTxn;
};
