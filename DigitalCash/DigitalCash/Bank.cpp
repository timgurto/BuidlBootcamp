#include "Bank.h"

void Bank::issueTo(PublicKey receiver) {
  m_coins.insert({Coin::CreateByIssuingTo(receiver)});
}

std::set<Coin> Bank::coinsOwnedBy(PublicKey owner) const {
  for (auto &coin : m_coins) {
    if (coin.currentOwner() == owner) return {coin};
  }
  return {};
}
