#include "Bank.h"

void Bank::issue(int, PublicKey) { m_AlicesBalance = 1000; }

int Bank::checkBalance(PublicKey) { return m_AlicesBalance; }
