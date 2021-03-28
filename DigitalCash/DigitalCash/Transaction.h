#pragma once

#include <iostream>
#include <string>

struct Transaction {
  std::string sender;
  std::string receiver;
  std::string signature;

  bool operator==(const Transaction& rhs) const;
  bool operator!=(const Transaction& rhs) const { return !(*this == rhs); }

  bool isSignatureValid() const;
};

std::istream& operator>>(std::istream& lhs, Transaction& rhs);
std::ostream& operator<<(std::ostream& lhs, const Transaction& rhs);
