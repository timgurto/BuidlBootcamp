#include "PublicKey.h"

#include <hex.h>

using namespace std::string_literals;

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

std::string PublicKey::toHexString() const {
  auto hexString = ""s;
  m_publicKey.Save(CryptoPP::HexEncoder(new CryptoPP::StringSink(hexString)));
  return hexString;
}

void PublicKey::fromHexString(const std::string &hexString) {
  auto key = ""s;
  CryptoPP::StringSource{
      hexString, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(key))};
  m_publicKey.Load(CryptoPP::StringSource(key, true));
  m_isEmpty = false;
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
  lhs << rhs.toHexString();
  return lhs;
}

std::istream &operator>>(std::istream &lhs, PublicKey &rhs) {
  auto hexString = ""s;
  lhs >> hexString;
  rhs.fromHexString(hexString);
  return lhs;
}
