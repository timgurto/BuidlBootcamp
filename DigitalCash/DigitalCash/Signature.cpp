#include "Signature.h"

#include <hex.h>

using namespace std::string_literals;

Signature::Signature(const std::string &sig) : m_signature(sig) {}

bool Signature::operator==(const Signature &rhs) const {
  return m_signature == rhs.m_signature;
}

bool Signature::operator!=(const Signature &rhs) const {
  return !(*this == rhs);
}

bool Signature::exists() const { return !m_signature.empty(); }

const unsigned char *Signature::address() const {
  return (const unsigned char *)&m_signature[0];
  // auto firstChar = &m_signature[0];

  // return static_cast<const unsigned char *>(firstChar);
}

const char &Signature::firstChar() const { return m_signature[0]; }

size_t Signature::size() const { return m_signature.size(); }

std::istream &operator>>(std::istream &lhs, Signature &rhs) {
  auto hex = ""s;
  lhs >> hex;
  CryptoPP::StringSource ss(
      hex, true,
      new CryptoPP::HexDecoder(new CryptoPP::StringSink(rhs.m_signature)));
  return lhs;
}

std::ostream &operator<<(std::ostream &lhs, const Signature &rhs) {
  auto hex = ""s;
  CryptoPP::StringSource ss(
      rhs.m_signature, true,
      new CryptoPP::HexEncoder(new CryptoPP::StringSink(hex)));

  lhs << hex;
  return lhs;
}
