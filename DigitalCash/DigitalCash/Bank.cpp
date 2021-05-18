#include "Bank.h"

Transaction Bank::issue(Currency amount, PublicKey) {
  m_AlicesBalance = amount;
  return {};
}

Currency Bank::checkBalance(PublicKey) { return m_AlicesBalance; }

void Bank::handleTransaction(const Transaction& tx) {}
