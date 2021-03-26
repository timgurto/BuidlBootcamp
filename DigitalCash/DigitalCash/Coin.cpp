#include "Coin.h"

bool Coin::isValid() const {
  if (m_transactions.front().sender != "Government") return false;
  for (const auto &txn : m_transactions)
    if (!txn.isSignatureValid()) return false;
  return true;
}
