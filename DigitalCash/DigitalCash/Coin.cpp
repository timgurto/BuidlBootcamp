#include "Coin.h"

#include <sstream>

bool Coin::operator==(const Coin &rhs) const {
  if (!numberOfTransactionsMatch(*this, rhs)) return false;

  for (auto i = 0; i != m_transactions.size(); ++i)
    if (m_transactions[i] != rhs.m_transactions[i]) return false;

  return true;
}

bool Coin::numberOfTransactionsMatch(const Coin &lhs, const Coin &rhs) {
  return lhs.m_transactions.size() == rhs.m_transactions.size();
}

Coin Coin::Deserialise(const std::string &serialisedCoin) {
  return Coin{serialisedCoin};
}

Coin::Coin(const std::string &serialisedCoin) {
  auto iss = std::istringstream{serialisedCoin};

  auto firstTransaction = Transaction{};
  std::getline(iss, firstTransaction.sender);
  std::getline(iss, firstTransaction.receiver);

  addTxn(firstTransaction);
}

std::string Coin::serialise() const {
  if (m_transactions.empty()) return {};

  auto oss = std::ostringstream{};
  oss << m_transactions.front().sender << std::endl
      << m_transactions.front().receiver;
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
