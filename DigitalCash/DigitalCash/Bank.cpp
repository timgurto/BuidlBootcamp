#include "Bank.h"

int Bank::issue(Currency amount, PublicKey) {
  m_AlicesBalance = amount;
  return {};
}

Currency Bank::checkBalance(PublicKey) { return m_AlicesBalance; }
