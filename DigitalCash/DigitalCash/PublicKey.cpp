#include "PublicKey.h"

PublicKey::PublicKey(const ECDSA::PublicKey &rawPublicKey)
    : m_publicKey(rawPublicKey) {
  m_isEmpty = false;
}

PublicKey PublicKey::ToBeReadInto() { return {}; }

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
  /*auto asString = std::string{};
  auto stringSink = CryptoPP::StringSink{asString};
  rhs.m_publicKey.DEREncode(stringSink);

  lhs << asString;*/

  return lhs;
}

std::istream &operator>>(std::istream &lhs, PublicKey &rhs) {
  /*auto asString = std::string{};
  lhs >> asString;

  CryptoPP::StringSource stringSource(asString, true);
  rhs.m_publicKey.BERDecode(stringSource);*/

  return lhs;
}
