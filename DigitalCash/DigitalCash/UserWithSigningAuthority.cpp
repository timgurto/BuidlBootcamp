
#include "UserWithSigningAuthority.h"

#include "Transaction.h"

void UserWithSigningAuthority::sign(class Transaction& transaction) const {
  transaction.m_signature = "Signed, " + name;
}
