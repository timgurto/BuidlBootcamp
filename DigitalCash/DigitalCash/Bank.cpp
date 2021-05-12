#include "Bank.h"

void Bank::issueTo(PublicKey receiver) {
  m_coins.insert({Coin::CreateByIssuingTo(receiver)});
}

std::set<Coin> Bank::coinsOwnedBy(PublicKey owner) const {
  auto coins = std::set<Coin>{};
  for (auto &coin : m_coins) {
    if (coin.currentOwner() == owner) coins.insert(coin);
  }
  return coins;
}

void Bank::observe(Coin toBeUpdated) {
  if (!toBeUpdated.isValid()) return;

  m_coins.erase(toBeUpdated);
  m_coins.insert(toBeUpdated);
}
