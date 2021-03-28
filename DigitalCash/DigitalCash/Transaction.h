#pragma once

#include <istream>
#include <string>

struct Transaction {
  std::string sender;
  std::string receiver;
  std::string signature;

  bool operator==(const Transaction& rhs) const {
    if (sender != rhs.sender) return false;
    if (receiver != rhs.receiver) return false;
    if (signature != rhs.signature) return false;

    return true;
  }

  bool operator!=(const Transaction& rhs) const { return !(*this == rhs); }

  bool isSignatureValid() const {
    const auto expectedSignature = "Signed, " + sender;
    return signature == expectedSignature;
  }
};

std::istream& operator>>(std::istream& lhs, Transaction& rhs);
