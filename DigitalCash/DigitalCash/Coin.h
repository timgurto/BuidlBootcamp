#pragma once

#include "Transaction.h"

class Coin {
 public:
  void addTxn(const Transaction &txn) { m_firstTxn = txn; }
  const Transaction &getFirstTxn() const { return m_firstTxn; }

  bool isValid() const {
    if (m_firstTxn.sender != "Government") return false;
    if (!m_firstTxn.isSignatureValid()) return false;
    return true;
  }

 private:
  Transaction m_firstTxn;
};
