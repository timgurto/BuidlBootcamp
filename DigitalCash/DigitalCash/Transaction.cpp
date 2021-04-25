#include "Transaction.h"

using namespace std::string_literals;

Transaction::Transaction(PublicKey sender, PublicKey receiver)
    : m_sender(sender), m_receiver(receiver) {
  auto msgStream = std::ostringstream{};
  msgStream << sender << std::endl << receiver;
  m_message = msgStream.str();
  m_signature = Signature{"unsigned"s};
}

bool Transaction::operator==(const Transaction& rhs) const {
  if (m_sender != rhs.m_sender) return false;
  if (m_receiver != rhs.m_receiver) return false;
  if (m_signature != rhs.m_signature) return false;

  return true;
}

std::istream& operator>>(std::istream& lhs, Transaction& rhs) {
  lhs >> rhs.m_sender >> rhs.m_receiver >> rhs.m_signature;
  return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const Transaction& rhs) {
  lhs << rhs.m_sender << std::endl;
  lhs << rhs.m_receiver << std::endl;
  lhs << rhs.m_signature;

  return lhs;
}

bool Transaction::isSignatureValid() const {
  if (!m_signature.exists()) return false;
  return m_sender.verifySignatureForMessage(m_signature, m_message);
}

Transaction Transaction::ReadFrom(std::istream& input) { return {input}; }

Transaction::Transaction(std::istream& input)
    : m_sender(PublicKey::ToBeReadInto()),
      m_receiver(PublicKey::ToBeReadInto()) {
  input >> *this;
}
