#pragma once

#include <string>

#include "WeakUser.h"

// Represents an address that can hold coin, and the authority to sign
// transactions sent from that address.
class UserWithSigningAuthority : public WeakUser {
 public:
  UserWithSigningAuthority(const std::string& name);
  void sign(class Transaction& transaction) const;
};
