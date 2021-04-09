#pragma once

#include "PublicKey.h"

// A public-private key pair that can sign transactions
class PrivateKey {
 public:
  PublicKey getPublicKey() const;
  Signature sign(const std::string &message) const;
  static PrivateKey Generate();

 private:
  PrivateKey();

  static CryptoPP::AutoSeededRandomPool rng;
  ECDSA::PrivateKey m_privateKey;
};
