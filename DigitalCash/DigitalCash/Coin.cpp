#include "Coin.h"

bool Coin::operator==(const Coin &rhs) const {
  if (m_transactions.size() != rhs.m_transactions.size()) return false;

  for (auto i = 0; i != m_transactions.size(); ++i)
    if (m_transactions[i] != rhs.m_transactions[i]) return false;

  return true;
}

Coin Coin::Deserialise(const std::string &serialisedCoin) { return {}; }

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
