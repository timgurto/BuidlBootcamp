#include "Coin.h"

#include <sstream>

bool Coin::operator==(const Coin &rhs) const {
  return numberOfTransactionsMatch(*this, rhs) && transactionsMatch(*this, rhs);
}

bool Coin::numberOfTransactionsMatch(const Coin &lhs, const Coin &rhs) {
  return lhs.m_transactions.size() == rhs.m_transactions.size();
}

bool Coin::transactionsMatch(const Coin &lhs, const Coin &rhs) {
  for (auto i = 0; i != lhs.m_transactions.size(); ++i)
    if (lhs.m_transactions[i] != rhs.m_transactions[i]) return false;
  return true;
}

Coin Coin::Deserialise(const std::string &serialisedCoin) {
  return Coin{serialisedCoin};
}

Coin::Coin(const std::string &serialisedCoin) {
  auto iss = std::istringstream{serialisedCoin};

  auto transaction = Transaction{};
  iss >> transaction;

  addTxn(transaction);

  if (!iss.eof()) {
    iss >> transaction;
    addTxn(transaction);
  }
}

std::string Coin::serialise() const {
  if (m_transactions.empty()) return {};

  auto oss = std::ostringstream{};
  oss << m_transactions.front();
  if (m_transactions.size() > 1) oss <<std::endl<< m_transactions[1] << std::endl;
  return oss.str();
}

bool Coin::isValid() const {
  return coinWasIssuedByTheGovernment() && allTransactionsHaveValidSignatures();
}

bool Coin::coinWasIssuedByTheGovernment() const {
  if (m_transactions.empty()) return true;

  const auto issuer = m_transactions.front().sender;
  return issuer == "Government";
}

bool Coin::allTransactionsHaveValidSignatures() const {
  for (const auto &txn : m_transactions)
    if (!txn.isSignatureValid()) return false;
  return true;
}
