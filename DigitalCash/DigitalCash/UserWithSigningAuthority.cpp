
#include "UserWithSigningAuthority.h"

#include <random>

UserWithSigningAuthority::UserWithSigningAuthority()
    : m_keys(PrivateKey::Generate()) {}

PublicKey UserWithSigningAuthority::getWeakVersion() const {
  return m_keys.getPublicKey();
}
