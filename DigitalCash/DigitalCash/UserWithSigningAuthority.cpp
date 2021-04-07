
#include "UserWithSigningAuthority.h"

#include <random>

#include "Transaction.h"

UserWithSigningAuthority UserWithSigningAuthority::AUTH_GOVERNMENT;
WeakUser UserWithSigningAuthority::WEAK_GOVERNMENT =
    AUTH_GOVERNMENT.getWeakVersion();

UserWithSigningAuthority::UserWithSigningAuthority() : m_id(rand()) {}

void UserWithSigningAuthority::sign(class Transaction& transaction) const {
  transaction.m_signature =
      transaction.m_sender + transaction.m_receiver + m_id;
}

WeakUser UserWithSigningAuthority::getWeakVersion() const { return m_id; }
