
#include "UserWithSigningAuthority.h"

#include "Transaction.h"

UserWithSigningAuthority::UserWithSigningAuthority(const std::string& name)
    : WeakUser({name}) {}

void UserWithSigningAuthority::sign(class Transaction& transaction) const {
  transaction.m_signature = "Signed, " + name;
}
