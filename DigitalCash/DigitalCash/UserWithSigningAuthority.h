#pragma once

/* Represents an address that can hold coin, and the authority to sign
 * transactions sent from that address. */
class UserWithSigningAuthority {
 public:
  std::string name;
  std::string signature;
};
