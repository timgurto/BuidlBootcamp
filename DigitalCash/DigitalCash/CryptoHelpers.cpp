#include "CryptoHelpers.h"

CryptoPP::AutoSeededRandomPool KeyPair::rng;

PublicKey::PublicKey(
    const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey
        &rawPublicKey)
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

PublicKey KeyPair::getPublicKey() const {
  auto publicKey =
      CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey{};
  m_privateKey.MakePublicKey(publicKey);
  return publicKey;
}

Signature KeyPair::sign(const std::string &message) const { return {}; }

KeyPair KeyPair::Generate() { return {}; }

KeyPair::KeyPair() {
  m_privateKey.Initialize(rng, CryptoPP::ASN1::secp256k1());
}
