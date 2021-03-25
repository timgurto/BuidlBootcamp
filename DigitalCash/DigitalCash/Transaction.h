#pragma once

#include <string>

struct Transaction {
  std::string sender;
  std::string receiver;
  std::string signature;

  bool isSignatureValid() const {
    const auto expectedSignature = "Signed, " + sender;
    return signature == expectedSignature;
  }
};
