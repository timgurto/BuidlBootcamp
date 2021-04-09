#include "PublicKey.h"

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

bool PublicKey::verifySignatureForMessage(const std::string &signature,
                                          const std::string &message) const {
  auto verifier =
      CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier{m_publicKey};
  return verifier.VerifyMessage(
      (const unsigned char *)&message[0], message.size(),
      (const unsigned char *)&signature[0], signature.size());
}

std::ostream &operator<<(std::ostream &lhs, const PublicKey &rhs) {
  return lhs;
}

std::istream &operator>>(std::istream &lhs, PublicKey &rhs) { return lhs; }
