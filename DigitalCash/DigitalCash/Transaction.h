#pragma once

#include <iostream>
#include <string>

#include "PublicKey.h"

class Transaction {
 public:
  Transaction() = delete;
  Transaction(PublicKey sender, PublicKey receiver);

  bool operator==(const Transaction& rhs) const;
  bool operator!=(const Transaction& rhs) const { return !(*this == rhs); }

  bool isSignatureValid() const;

  static Transaction ReadFrom(std::istream& input);

  PublicKey m_sender;
  PublicKey m_receiver;
  std::string m_message;
  Signature m_signature;

 private:
  Transaction(std::istream& input);  // Used by ReadFrom()
};

std::istream& operator>>(std::istream& lhs, Transaction& rhs);
std::ostream& operator<<(std::ostream& lhs, const Transaction& rhs);
