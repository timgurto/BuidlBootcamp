#include "Transaction.h"

std::istream& operator>>(std::istream& lhs, Transaction& rhs) {
  std::getline(lhs, rhs.sender);
  std::getline(lhs, rhs.receiver);

  return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const Transaction& rhs) {
  lhs << rhs.sender << std::endl << rhs.receiver;

  return lhs;
}
