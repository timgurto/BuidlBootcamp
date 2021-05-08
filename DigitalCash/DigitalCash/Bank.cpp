#include "Bank.h"

void Bank::issueTo(PublicKey receiver) {
  m_coins = {Coin::CreateEmptyForTesting()};
}

std::set<Coin> Bank::coinsOwnedBy(PublicKey owner) const { return {m_coins}; }
