
#include "UserWithSigningAuthority.h"

#include <random>

UserWithSigningAuthority::UserWithSigningAuthority()
    : m_keys(PrivateKey::Generate()) {}

PublicKey UserWithSigningAuthority::getWeakVersion() const {
  return m_keys.getPublicKey();
}

void UserWithSigningAuthority::signInput(Transaction& tx, Index) {
  tx.inputs[0].signature = Signature{""s};
}
