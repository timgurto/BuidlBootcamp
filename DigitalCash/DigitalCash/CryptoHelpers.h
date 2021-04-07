#pragma once

#include <string>

#include "eccrypto.h"
#include "oids.h"
#include "osrng.h"

// Wrappers for the ECDSA functionality of Crypto++

using Signature = std::string;

// A public key.  Enough to send this person money, or to verify that a
// transaction was signed by him.
class PublicKey {
 public:
  bool operator==(const PublicKey &rhs) const;
  bool operator!=(const PublicKey &rhs) const;
  friend std::ostream &operator<<(std::ostream &lhs, const PublicKey &rhs);
  friend std::istream &operator>>(std::istream &lhs, PublicKey &rhs);

  bool verifySignatureForMessage(const std::string &message) const;
};

// A public-private key pair that can sign transactions
class KeyPair {
 public:
  PublicKey getPublicKey() const;
  Signature sign(const std::string &message) const;
  static KeyPair Generate();

 private:
  KeyPair();
};
