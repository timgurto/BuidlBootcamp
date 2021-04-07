#pragma once

#include <string>

#include "WeakUser.h"

// Represents an address that can hold coin, and the authority to sign
// transactions sent from that address.
class UserWithSigningAuthority {
 public:
  UserWithSigningAuthority();
  void sign(class Transaction& transaction) const;
  WeakUser getWeakVersion() const;

  static UserWithSigningAuthority AUTH_GOVERNMENT;
  static WeakUser WEAK_GOVERNMENT;

 private:
  int m_id;
};
