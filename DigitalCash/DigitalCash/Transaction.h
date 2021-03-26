#pragma once

#include <string>

struct Transaction {
  std::string sender;
  std::string receiver;
  std::string signature;

  bool operator==(const Transaction& rhs) const {
    return sender == rhs.sender && receiver == rhs.receiver;
  }

  bool operator!=(const Transaction& rhs) const { return !(*this == rhs); }

  bool isSignatureValid() const {
    const auto expectedSignature = "Signed, " + sender;
    return signature == expectedSignature;
  }
};
