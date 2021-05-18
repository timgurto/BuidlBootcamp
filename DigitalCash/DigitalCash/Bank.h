#pragma once

#include "PublicKey.h"
#include "Transaction.h"

class Bank {
 public:
  Transaction issue(Currency amount, PublicKey);

  Currency checkBalance(PublicKey);

  void handleTransaction(const Transaction& tx);

 private:
  Currency m_AlicesBalance{0};
};
