#pragma once

#include <iostream>
#include <string>

#include "PublicKey.h"

class Transaction {
 public:
  Transaction() = default;
  Transaction(PublicKey sender, PublicKey receiver);

  bool operator==(const Transaction& rhs) const;
  bool operator!=(const Transaction& rhs) const { return !(*this == rhs); }

  bool isSignatureValid() const;

  PublicKey m_sender;
  PublicKey m_receiver;
  std::string m_message;
  Signature m_signature;
};

std::istream& operator>>(std::istream& lhs, Transaction& rhs);
std::ostream& operator<<(std::ostream& lhs, const Transaction& rhs);
