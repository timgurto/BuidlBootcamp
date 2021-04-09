#include "PublicKey.h"

PublicKey::PublicKey(const ECDSA::PublicKey &rawPublicKey)
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
  auto verifier = ECDSA::Verifier{m_publicKey};
  const auto messageAddress = (const unsigned char *)&message[0];
  const auto signatureAddress = (unsigned char *)&signature[0];
  return verifier.VerifyMessage(messageAddress, message.size(),
                                signatureAddress, signature.size());
}

std::ostream &operator<<(std::ostream &lhs, const PublicKey &rhs) {
  return lhs;
}

std::istream &operator>>(std::istream &lhs, PublicKey &rhs) { return lhs; }
