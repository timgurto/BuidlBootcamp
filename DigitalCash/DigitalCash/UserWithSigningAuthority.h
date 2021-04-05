#pragma once

#include <string>

/* Represents an address that can hold coin, and the authority to sign
 * transactions sent from that address. */
class UserWithSigningAuthority {
 public:
  std::string name;

  void sign(class Transaction& transaction) const;
};
