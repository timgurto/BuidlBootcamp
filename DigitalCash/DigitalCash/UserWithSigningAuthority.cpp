
#include "UserWithSigningAuthority.h"

#include "Transaction.h"

UserWithSigningAuthority::UserWithSigningAuthority(const std::string& name)
    : name(name) {}

void UserWithSigningAuthority::sign(class Transaction& transaction) const {
  transaction.m_signature =
      transaction.m_sender + transaction.m_receiver + name;
}
