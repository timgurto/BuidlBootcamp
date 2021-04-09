#include "PrivateKey.h"

CryptoPP::AutoSeededRandomPool PrivateKey::rng;

PublicKey PrivateKey::getPublicKey() const {
  auto publicKey = ECDSA::PublicKey{};
  m_privateKey.MakePublicKey(publicKey);
  return publicKey;
}

Signature PrivateKey::sign(const std::string &message) const {
  auto signer = ECDSA::Signer{m_privateKey};
  std::string signature(signer.MaxSignatureLength(), '\0');
  auto signatureLength =
      signer.SignMessage(rng, (const unsigned char *)&message[0],
                         message.size(), (unsigned char *)&signature[0]);
  signature.resize(signatureLength);
  return signature;
}

PrivateKey PrivateKey::Generate() { return {}; }

PrivateKey::PrivateKey() {
  m_privateKey.Initialize(rng, CryptoPP::ASN1::secp256k1());
}
