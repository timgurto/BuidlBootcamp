#pragma once

#include <set>

#include "Coin.h"
#include "UserWithSigningAuthority.h"

class Bank {
 public:
  void issueTo(PublicKey receiver);
  std::set<Coin> coinsOwnedBy(PublicKey owner) const;
  void observe(Coin toBeUpdated);
  bool isCoinValid(Coin coin) const;
  /**/ bool coinWasIssuedByThisBank(Coin coin) const;

  PublicKey getPublicKey() const { return m_keys.getWeakVersion(); }

 private:
  std::set<Coin> m_coins;
  UserWithSigningAuthority m_keys;
};
