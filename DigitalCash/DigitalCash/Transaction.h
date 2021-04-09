#pragma once

#include <iostream>
#include <string>

#include "CryptoHelpers.h"

class Transaction {
 public:
  Transaction() = default;
  Transaction(PublicKeyWrapper sender, PublicKeyWrapper receiver);

  bool operator==(const Transaction& rhs) const;
  bool operator!=(const Transaction& rhs) const { return !(*this == rhs); }

  bool isSignatureValid() const;

  PublicKeyWrapper m_sender;
  PublicKeyWrapper m_receiver;
  std::string m_message;
  Signature m_signature;
};

std::istream& operator>>(std::istream& lhs, Transaction& rhs);
std::ostream& operator<<(std::ostream& lhs, const Transaction& rhs);
