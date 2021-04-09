#include "CryptoHelpers.h"

CryptoPP::AutoSeededRandomPool PrivateKeyWrapper::rng;

PublicKeyWrapper::PublicKeyWrapper(
    const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey
        &rawPublicKey)
    : m_publicKey(rawPublicKey) {
  m_isEmpty = false;
}

bool PublicKeyWrapper::operator==(const PublicKeyWrapper &rhs) const {
  if (m_isEmpty != rhs.m_isEmpty) return false;
  if (m_isEmpty) return true;  // Both are empty
  return m_publicKey == rhs.m_publicKey;
}

bool PublicKeyWrapper::operator!=(const PublicKeyWrapper &rhs) const {
  return !(*this == rhs);
}

bool PublicKeyWrapper::verifySignatureForMessage(
    const std::string &message) const {
  return {};
}

std::ostream &operator<<(std::ostream &lhs, const PublicKeyWrapper &rhs) {
  return lhs;
}

std::istream &operator>>(std::istream &lhs, PublicKeyWrapper &rhs) {
  return lhs;
}

PublicKeyWrapper PrivateKeyWrapper::getPublicKey() const {
  auto publicKey =
      CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey{};
  m_privateKey.MakePublicKey(publicKey);
  return publicKey;
}

Signature PrivateKeyWrapper::sign(const std::string &message) const {
  return {};
}

PrivateKeyWrapper PrivateKeyWrapper::Generate() { return {}; }

PrivateKeyWrapper::PrivateKeyWrapper() {
  m_privateKey.Initialize(rng, CryptoPP::ASN1::secp256k1());
}
