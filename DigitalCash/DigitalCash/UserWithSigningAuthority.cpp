
#include "UserWithSigningAuthority.h"

#include <random>

#include "Transfer.h"

UserWithSigningAuthority::UserWithSigningAuthority()
    : m_keys(PrivateKey::Generate()) {}

void UserWithSigningAuthority::sign(class Transfer& transaction) const {
  transaction.m_signature = m_keys.sign(transaction.generateMessage());
}

PublicKey UserWithSigningAuthority::getWeakVersion() const {
  return m_keys.getPublicKey();
}
