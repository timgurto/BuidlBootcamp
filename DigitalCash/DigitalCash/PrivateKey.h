#pragma once

#include "PublicKey.h"

using ECDSAPrivateKey =
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey;

// A public-private key pair that can sign transactions
class PrivateKey {
 public:
  PublicKey getPublicKey() const;
  Signature sign(const std::string &message) const;
  static PrivateKey Generate();

 private:
  PrivateKey();

  static CryptoPP::AutoSeededRandomPool rng;
  ECDSAPrivateKey m_privateKey;
};
