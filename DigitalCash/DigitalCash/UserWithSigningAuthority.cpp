
#include "UserWithSigningAuthority.h"

#include <random>

UserWithSigningAuthority::UserWithSigningAuthority()
    : m_keys(PrivateKey::Generate()) {}

PublicKey UserWithSigningAuthority::getWeakVersion() const {
  return m_keys.getPublicKey();
}

void UserWithSigningAuthority::signInput(Transaction& tx, Index i) const {
  const auto message = tx.getMessageForInput(i);
  tx.inputs[i].signature = m_keys.sign(message);
}
