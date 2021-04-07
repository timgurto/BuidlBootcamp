#include "Transaction.h"

Transaction::Transaction(WeakUser sender, WeakUser receiver)
    : m_sender(sender), m_receiver(receiver) {}

bool Transaction::operator==(const Transaction& rhs) const {
  if (m_sender != rhs.m_sender) return false;
  if (m_receiver != rhs.m_receiver) return false;
  if (m_signature != rhs.m_signature) return false;

  return true;
}

std::istream& operator>>(std::istream& lhs, Transaction& rhs) {
  std::getline(lhs, rhs.m_sender);
  std::getline(lhs, rhs.m_receiver);
  std::getline(lhs, rhs.m_signature);

  return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const Transaction& rhs) {
  lhs << rhs.m_sender << std::endl;
  lhs << rhs.m_receiver << std::endl;
  lhs << rhs.m_signature;

  return lhs;
}

bool Transaction::isSignatureValid() const {
  const auto expectedSignature = m_sender + m_receiver + m_sender;
  return m_signature == expectedSignature;
}
