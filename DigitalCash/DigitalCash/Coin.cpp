#include "Coin.h"

bool Coin::isValid() const {
  if (!coinWasIssuedByTheGovernment()) return false;
  for (const auto &txn : m_transactions)
    if (!txn.isSignatureValid()) return false;
  return true;
}

bool Coin::coinWasIssuedByTheGovernment() const {
  return m_transactions.front().sender == "Government";
}
