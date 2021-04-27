#pragma once

#include <string>

#include "PrivateKey.h"

// Represents an address that can hold coin, and the authority to sign
// transactions sent from that address.
class UserWithSigningAuthority {
 public:
  UserWithSigningAuthority();
  void sign(class Transfer& transaction) const;
  PublicKey getWeakVersion() const;

  static UserWithSigningAuthority& authGovernment();
  static PublicKey weakGovernment();

 private:
  PrivateKey m_keys;
};
