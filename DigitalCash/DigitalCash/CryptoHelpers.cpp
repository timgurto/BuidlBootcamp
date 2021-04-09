#include "CryptoHelpers.h"

CryptoPP::AutoSeededRandomPool PrivateKey::rng;

PublicKey::PublicKey(const ECDSAPublicKey &rawPublicKey)
    : m_publicKey(rawPublicKey) {
  m_isEmpty = false;
}

bool PublicKey::operator==(const PublicKey &rhs) const {
  if (m_isEmpty != rhs.m_isEmpty) return false;
  if (m_isEmpty) return true;  // Both are empty
  return m_publicKey == rhs.m_publicKey;
}

bool PublicKey::operator!=(const PublicKey &rhs) const {
  return !(*this == rhs);
}

bool PublicKey::verifySignatureForMessage(const std::string &message) const {
  return {};
}

std::ostream &operator<<(std::ostream &lhs, const PublicKey &rhs) {
  return lhs;
}

std::istream &operator>>(std::istream &lhs, PublicKey &rhs) { return lhs; }

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
