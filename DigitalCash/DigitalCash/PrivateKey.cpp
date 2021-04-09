#include "PrivateKey.h"

CryptoPP::AutoSeededRandomPool PrivateKey::rng;

PublicKey PrivateKey::getPublicKey() const {
  auto publicKey = ECDSAPublicKey{};
  m_privateKey.MakePublicKey(publicKey);
  return publicKey;
}

Signature PrivateKey::sign(const std::string &message) const { return {}; }

PrivateKey PrivateKey::Generate() { return {}; }

PrivateKey::PrivateKey() {
  m_privateKey.Initialize(rng, CryptoPP::ASN1::secp256k1());
}
