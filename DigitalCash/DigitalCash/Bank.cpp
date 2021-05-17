#include "Bank.h"

void Bank::issue(Currency amount, PublicKey) { m_AlicesBalance = amount; }

Currency Bank::checkBalance(PublicKey) { return m_AlicesBalance; }
