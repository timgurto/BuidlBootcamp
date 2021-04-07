
#include "UserWithSigningAuthority.h"

#include <random>

#include "CryptoHelpers.h"
#include "Transaction.h"

UserWithSigningAuthority UserWithSigningAuthority::AUTH_GOVERNMENT;
PublicKey UserWithSigningAuthority::WEAK_GOVERNMENT =
    AUTH_GOVERNMENT.getWeakVersion();

UserWithSigningAuthority::UserWithSigningAuthority()
    : m_keys(KeyPair::Generate()) {}

void UserWithSigningAuthority::sign(class Transaction& transaction) const {
  transaction.m_signature = m_keys.sign(transaction.m_message);
}

PublicKey UserWithSigningAuthority::getWeakVersion() const {
  return m_keys.getPublicKey();
}
