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
  auto numTransactions = 0;

  auto iss = std::istringstream{serialisedCoin};
  iss >> numTransactions;

  for (auto i = 0; i != numTransactions; ++i) readAndAddTransaction(iss);
}

void Coin::readAndAddTransaction(std::istringstream &serialisedCoin) {
  auto nextTransaction = Transaction::ReadFrom(serialisedCoin);
  addTransaction(nextTransaction);
}

std::string Coin::serialise() const {
  auto oss = std::ostringstream{};

  oss << m_transactions.size() << std::endl;

  for (const auto &transaction : m_transactions)
    oss << transaction << std::endl;

  return oss.str();
}

bool Coin::isValid() const {
  return coinWasIssuedByTheGovernment() &&
         allTransactionsHaveValidSignatures() && eachSpenderWasTheOwner();
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

bool Coin::eachSpenderWasTheOwner() const {
  auto ownerAtThatPoint = UserWithSigningAuthority::weakGovernment();

  for (const auto &txn : m_transactions) {
    if (txn.m_sender != ownerAtThatPoint) return false;
    ownerAtThatPoint = txn.m_receiver;
  }

  return true;
}
