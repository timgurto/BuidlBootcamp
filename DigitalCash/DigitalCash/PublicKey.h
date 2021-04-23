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
  PublicKey(const ECDSA::PublicKey &rawPublicKey);
  static PublicKey ToBeReadInto();

  bool operator==(const PublicKey &rhs) const;
  bool operator!=(const PublicKey &rhs) const;
  friend std::ostream &operator<<(std::ostream &lhs, const PublicKey &rhs);
  friend std::istream &operator>>(std::istream &lhs, PublicKey &rhs);

  std::string toHexString() const;
  void fromHexString(const std::string &hexString);

  bool verifySignatureForMessage(const std::string &signature,
                                 const std::string &message) const;

 private:
  PublicKey() {}  // Used by ToBeReadInto()

  bool m_isEmpty = true;
  ECDSA::PublicKey m_publicKey;
};
