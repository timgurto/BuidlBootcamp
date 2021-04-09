#include "Coin.h"

#include <sstream>

#include "UserWithSigningAuthority.h"

bool Coin::operator==(const Coin &rhs) const {
  return m_transactions == rhs.m_transactions;
}

Coin Coin::Deserialise(const std::string &serialisedCoin) {
  return Coin{serialisedCoin};
}

Coin::Coin(const std::string &serialisedCoin) {
  auto iss = std::istringstream{serialisedCoin};
  while (!iss.eof()) readAndAddTransaction(iss);
}

void Coin::readAndAddTransaction(std::istringstream &serialisedCoin) {
  auto nextTransaction = Transaction{};
  serialisedCoin >> nextTransaction;
  addTransaction(nextTransaction);
}

std::string Coin::serialise() const {
  if (m_transactions.empty()) return {};

  auto oss = std::ostringstream{};

  for (auto i = 0; i != m_transactions.size(); ++i) {
    if (i != 0) oss << std::endl;
    oss << m_transactions[i];
  }

  return oss.str();
}

bool Coin::isValid() const {
  return coinWasIssuedByTheGovernment() && allTransactionsHaveValidSignatures();
}

bool Coin::coinWasIssuedByTheGovernment() const {
  if (m_transactions.empty()) return true;

  const auto issuer = m_transactions.front().m_sender;
  const auto expectedIssuer = UserWithSigningAuthority::weakGovernment();
  return issuer == expectedIssuer;
}

bool Coin::allTransactionsHaveValidSignatures() const {
  for (const auto &txn : m_transactions)
    if (!txn.isSignatureValid()) return false;
  return true;
}
