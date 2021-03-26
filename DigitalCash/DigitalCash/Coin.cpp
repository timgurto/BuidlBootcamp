#include "Coin.h"

bool Coin::operator==(const Coin &rhs) const {
  return m_transactions.size() == rhs.m_transactions.size();
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
