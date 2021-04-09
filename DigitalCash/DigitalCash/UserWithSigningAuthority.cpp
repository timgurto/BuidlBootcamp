
#include "UserWithSigningAuthority.h"

#include <random>

#include "Transaction.h"

UserWithSigningAuthority::UserWithSigningAuthority()
    : m_keys(PrivateKey::Generate()) {}

UserWithSigningAuthority& UserWithSigningAuthority::authGovernment() {
  static UserWithSigningAuthority* pAuthGovernment =
      new UserWithSigningAuthority;
  return *pAuthGovernment;
}

PublicKey UserWithSigningAuthority::weakGovernment() {
  return authGovernment().getWeakVersion();
}

void UserWithSigningAuthority::sign(class Transaction& transaction) const {
  transaction.m_signature = m_keys.sign(transaction.m_message);
}

PublicKey UserWithSigningAuthority::getWeakVersion() const {
  return m_keys.getPublicKey();
}
