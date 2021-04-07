#pragma once

#include <string>

#include "WeakUser.h"

// Represents an address that can hold coin, and the authority to sign
// transactions sent from that address.
class UserWithSigningAuthority {
 public:
  UserWithSigningAuthority(const std::string& name);
  void sign(class Transaction& transaction) const;
  WeakUser getWeakVersion() const;

 private:
  std::string m_name;
};
