#include "Transaction.h"

Transaction::Transaction(PublicKey sender, PublicKey receiver)
    : m_sender(sender), m_receiver(receiver) {
  auto msgStream = std::ostringstream{};
  msgStream << sender << std::endl << receiver;
  m_message = msgStream.str();
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
  if (m_signature.empty()) return false;
  return m_sender.verifySignatureForMessage(m_signature, m_message);
}
