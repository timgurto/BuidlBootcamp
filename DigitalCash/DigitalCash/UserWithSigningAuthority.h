#pragma once

#include <string>

#include "PrivateKey.h"

// Represents an address that can hold coin, and the authority to sign
// transactions sent from that address.
class UserWithSigningAuthority {
 public:
  UserWithSigningAuthority();
  PublicKey getWeakVersion() const;

 private:
  PrivateKey m_keys;
};
