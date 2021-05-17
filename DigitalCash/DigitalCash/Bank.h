#pragma once

#include "PublicKey.h"

class Bank {
 public:
  void issue(int, PublicKey);

  int checkBalance(PublicKey);
};
