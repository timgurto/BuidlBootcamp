#include "Bank.h"

void Bank::issueTo(PublicKey receiver) {
  m_coins = {Coin::CreateByIssuingTo(receiver)};
}

std::set<Coin> Bank::coinsOwnedBy(PublicKey owner) const {
  if (m_coins.empty()) return {};
  auto firstCoin = *m_coins.begin();
  if (firstCoin.currentOwner() == owner)
    return {firstCoin};
  else
    return {};
}
