#pragma once

#include <string>

#include "CryptoHelpers.h"

// Represents an address that can hold coin, and the authority to sign
// transactions sent from that address.
class UserWithSigningAuthority {
 public:
  UserWithSigningAuthority();
  void sign(class Transaction& transaction) const;
  PublicKey getWeakVersion() const;

  static UserWithSigningAuthority AUTH_GOVERNMENT;
  static PublicKey WEAK_GOVERNMENT;

 private:
  int m_id;

  KeyPair m_keys;
};
