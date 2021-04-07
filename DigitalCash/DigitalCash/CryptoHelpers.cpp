#include "CryptoHelpers.h"

bool PublicKey::operator==(const PublicKey &rhs) const { return {}; }

bool PublicKey::operator!=(const PublicKey &rhs) const { return {}; }

bool PublicKey::verifySignatureForMessage(const std::string &message) const {
  return {};
}

std::ostream &operator<<(std::ostream &lhs, const PublicKey &rhs) {
  return lhs;
}

std::istream &operator>>(std::istream &lhs, PublicKey &rhs) { return lhs; }

PublicKey KeyPair::getPublicKey() const { return {}; }

Signature KeyPair::sign(const std::string &message) const { return {}; }

KeyPair KeyPair::Generate() { return {}; }

KeyPair::KeyPair() {}
