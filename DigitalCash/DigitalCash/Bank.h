#pragma once

#include "PublicKey.h"

class Bank {
 public:
  void issue(int amount, PublicKey);

  int checkBalance(PublicKey);

 private:
  int m_AlicesBalance{0};
};
