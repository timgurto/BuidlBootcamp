#pragma once

#include "PublicKey.h"
#include "Transaction.h"

using Currency = unsigned;

class Bank {
 public:
  Transaction issue(Currency amount, PublicKey);

  Currency checkBalance(PublicKey);

 private:
  Currency m_AlicesBalance{0};
};
