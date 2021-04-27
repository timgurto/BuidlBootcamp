#include "Transfer.h"

using namespace std::string_literals;

Transfer::Transfer(PublicKey sender, PublicKey receiver)
    : m_sender(sender), m_receiver(receiver) {
  m_signature = Signature{"unsigned"s};
}

bool Transfer::operator==(const Transfer& rhs) const {
  if (m_sender != rhs.m_sender) return false;
  if (m_receiver != rhs.m_receiver) return false;
  if (m_signature != rhs.m_signature) return false;

  return true;
}

std::istream& operator>>(std::istream& lhs, Transfer& rhs) {
  lhs >> rhs.m_sender >> rhs.m_receiver >> rhs.m_signature;
  return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const Transfer& rhs) {
  lhs << rhs.m_sender << std::endl;
  lhs << rhs.m_receiver << std::endl;
  lhs << rhs.m_signature;

  return lhs;
}

bool Transfer::isSignatureValid() const {
  if (!m_signature.exists()) return false;
  return m_sender.verifySignatureForMessage(m_signature, generateMessage());
}

Transfer::Message Transfer::generateMessage() const {
  auto oss = std::ostringstream{};
  oss << m_sender << std::endl << m_receiver;
  return oss.str();
}

Transfer Transfer::ReadFrom(std::istream& input) { return {input}; }

Transfer::Transfer(std::istream& input)
    : m_sender(PublicKey::ToBeReadInto()),
      m_receiver(PublicKey::ToBeReadInto()) {
  input >> *this;
}
