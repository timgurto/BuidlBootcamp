#pragma once

#include <string>

#include "eccrypto.h"
#include "oids.h"
#include "osrng.h"

// Wrappers for the ECDSA functionality of Crypto++

using Signature = std::string;

// A public key.  Enough to send this person money, or to verify that a
// transaction was signed by him.
class PublicKeyWrapper {
 public:
  PublicKeyWrapper() =
      default;  // Uninitialised is supported to accommodate simpler tests
  PublicKeyWrapper(
      const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey
          &rawPublicKey);
  bool operator==(const PublicKeyWrapper &rhs) const;
  bool operator!=(const PublicKeyWrapper &rhs) const;
  friend std::ostream &operator<<(std::ostream &lhs,
                                  const PublicKeyWrapper &rhs);
  friend std::istream &operator>>(std::istream &lhs, PublicKeyWrapper &rhs);

  bool verifySignatureForMessage(const std::string &message) const;

 private:
  bool m_isEmpty = true;
  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey m_publicKey;
};

// A public-private key pair that can sign transactions
class PrivateKeyWrapper {
 public:
  PublicKeyWrapper getPublicKey() const;
  Signature sign(const std::string &message) const;
  static PrivateKeyWrapper Generate();

 private:
  PrivateKeyWrapper();

  static CryptoPP::AutoSeededRandomPool rng;
  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey m_privateKey;
};
