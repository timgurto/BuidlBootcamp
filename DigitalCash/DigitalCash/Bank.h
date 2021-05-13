#pragma once

#include <set>

#include "Coin.h"
#include "PublicKey.h"

class Bank {
 public:
  void issueTo(PublicKey receiver);
  std::set<Coin> coinsOwnedBy(PublicKey owner) const;
  void observe(Coin toBeUpdated);
  bool isCoinValid(Coin coin) const;
  /**/ bool coinWasIssuedByTheGovernment(Coin coin) const;

 private:
  std::set<Coin> m_coins;
};
