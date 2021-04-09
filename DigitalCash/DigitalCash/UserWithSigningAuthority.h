#pragma once

#include <string>

#include "CryptoHelpers.h"

// Represents an address that can hold coin, and the authority to sign
// transactions sent from that address.
class UserWithSigningAuthority {
 public:
  UserWithSigningAuthority();
  void sign(class Transaction& transaction) const;
  PublicKeyWrapper getWeakVersion() const;

  static UserWithSigningAuthority& authGovernment();
  static PublicKeyWrapper weakGovernment();

 private:
  PrivateKeyWrapper m_keys;
};
