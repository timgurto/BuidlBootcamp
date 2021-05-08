#pragma once

#include <set>

#include "Coin.h"
#include "PublicKey.h"

class Bank {
 public:
  void issueTo(PublicKey receiver) {
    m_coins = {Coin::CreateEmptyForTesting()};
  }
  std::set<Coin> coinsOwnedBy(PublicKey owner) { return {m_coins}; }

 private:
  std::set<Coin> m_coins;
};
