
#include "UserWithSigningAuthority.h"

#include "Transaction.h"

UserWithSigningAuthority::UserWithSigningAuthority(int id) : m_id(id) {}

void UserWithSigningAuthority::sign(class Transaction& transaction) const {
  transaction.m_signature =
      transaction.m_sender + transaction.m_receiver + m_id;
}

WeakUser UserWithSigningAuthority::getWeakVersion() const { return m_id; }
