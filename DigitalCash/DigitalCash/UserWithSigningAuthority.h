#pragma once

#include <string>

#include "PrivateKey.h"
#include "Transaction.h"

// Represents an address that can hold coin, and the authority to sign
// transactions sent from that address.
class UserWithSigningAuthority {
 public:
  UserWithSigningAuthority();
  PublicKey getWeakVersion() const;
  void signInput(Transaction& tx, Index);

 private:
  PrivateKey m_keys;
};
