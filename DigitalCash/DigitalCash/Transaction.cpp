#include "Transaction.h"

bool Transaction::operator==(const Transaction& rhs) const {
  if (sender != rhs.sender) return false;
  if (receiver != rhs.receiver) return false;
  if (signature != rhs.signature) return false;

  return true;
}

std::istream& operator>>(std::istream& lhs, Transaction& rhs) {
  std::getline(lhs, rhs.sender);
  std::getline(lhs, rhs.receiver);

  return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const Transaction& rhs) {
  lhs << rhs.sender << std::endl << rhs.receiver;

  return lhs;
}

bool Transaction::isSignatureValid() const {
  const auto expectedSignature = "Signed, " + sender;
  return signature == expectedSignature;
}
