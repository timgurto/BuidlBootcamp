#include "Bank.h"

#include "UserWithSigningAuthority.h"

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
  if (!isCoinValid(toBeUpdated)) return;

  m_coins.erase(toBeUpdated);
  m_coins.insert(toBeUpdated);
}

bool Bank::isCoinValid(Coin coin) const {
  if (coin.isEmpty()) return true;

  return coinWasIssuedByTheGovernment(coin) &&
         coin.allTransactionsHaveValidSignatures() &&
         coin.eachSpenderWasTheOwner();
}

bool Bank::coinWasIssuedByTheGovernment(Coin coin) const {
  const auto expectedIssuer = UserWithSigningAuthority::weakGovernment();
  return coin.issuer() == expectedIssuer;
}
