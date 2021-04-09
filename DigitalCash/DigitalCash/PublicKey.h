#pragma once

#include "eccrypto.h"
#include "oids.h"
#include "osrng.h"

using ECDSA = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>;
using Signature = std::string;

// A public key.  Enough to send this person money, or to verify that a
// transaction was signed by him.
class PublicKey {
 public:
  PublicKey() =
      default;  // Uninitialised is supported to accommodate simpler tests
  PublicKey(const ECDSA::PublicKey &rawPublicKey);
  bool operator==(const PublicKey &rhs) const;
  bool operator!=(const PublicKey &rhs) const;
  friend std::ostream &operator<<(std::ostream &lhs, const PublicKey &rhs);
  friend std::istream &operator>>(std::istream &lhs, PublicKey &rhs);

  bool verifySignatureForMessage(const std::string &signature,
                                 const std::string &message) const;

 private:
  bool m_isEmpty = true;
  ECDSA::PublicKey m_publicKey;
};
