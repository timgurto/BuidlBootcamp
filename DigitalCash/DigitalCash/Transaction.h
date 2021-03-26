#pragma once

#include <string>

struct Transaction {
  std::string sender;
  std::string receiver;
  std::string signature;

  bool operator!=(const Transaction& rhs) const { return true; }

  bool isSignatureValid() const {
    const auto expectedSignature = "Signed, " + sender;
    return signature == expectedSignature;
  }
};
