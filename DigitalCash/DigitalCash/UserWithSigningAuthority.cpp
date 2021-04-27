
#include "UserWithSigningAuthority.h"

#include <random>

#include "Transfer.h"

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

void UserWithSigningAuthority::sign(class Transfer& transaction) const {
  transaction.m_signature = m_keys.sign(transaction.generateMessage());
}

PublicKey UserWithSigningAuthority::getWeakVersion() const {
  return m_keys.getPublicKey();
}
