#pragma once

#include "PublicKey.h"

using Currency = unsigned;

class Bank {
 public:
  void issue(Currency amount, PublicKey);

  Currency checkBalance(PublicKey);

 private:
  Currency m_AlicesBalance{0};
};
