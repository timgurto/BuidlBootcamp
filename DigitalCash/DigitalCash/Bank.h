#pragma once

#include "PublicKey.h"

using Currency = unsigned;

class Bank {
 public:
  int issue(Currency amount, PublicKey);

  Currency checkBalance(PublicKey);

 private:
  Currency m_AlicesBalance{0};
};
