#pragma once

#include <set>

#include "Coin.h"
#include "PublicKey.h"

class Bank {
 public:
  void issueTo(PublicKey receiver);
  std::set<Coin> coinsOwnedBy(PublicKey owner) const;

 private:
  std::set<Coin> m_coins;
};
