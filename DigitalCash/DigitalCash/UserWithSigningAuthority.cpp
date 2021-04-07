
#include "UserWithSigningAuthority.h"

#include "Transaction.h"

UserWithSigningAuthority::UserWithSigningAuthority(const std::string& name)
    : m_name(name) {}

void UserWithSigningAuthority::sign(class Transaction& transaction) const {
  transaction.m_signature =
      transaction.m_sender + transaction.m_receiver + m_name;
}

WeakUser UserWithSigningAuthority::getWeakVersion() const { return m_name; }
