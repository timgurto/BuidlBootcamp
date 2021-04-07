#pragma once

#include <string>

#include "WeakUser.h"

// Represents an address that can hold coin, and the authority to sign
// transactions sent from that address.
class UserWithSigningAuthority {
 public:
  UserWithSigningAuthority(int id);
  void sign(class Transaction& transaction) const;
  WeakUser getWeakVersion() const;

 private:
  int m_id;
};
