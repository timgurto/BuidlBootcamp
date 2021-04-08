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

  static UserWithSigningAuthority& authGovernment();
  static PublicKey weakGovernment();

 private:
  int m_id;

  KeyPair m_keys;
};
