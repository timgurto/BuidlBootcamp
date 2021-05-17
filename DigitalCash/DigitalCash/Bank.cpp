#include "Bank.h"

void Bank::issue(int amount, PublicKey) { m_AlicesBalance = amount; }

int Bank::checkBalance(PublicKey) { return m_AlicesBalance; }
