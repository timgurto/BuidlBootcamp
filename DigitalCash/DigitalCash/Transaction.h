#pragma once

#include <iostream>
#include <string>

class Transaction {
 public:
  Transaction() = default;
  Transaction(const std::string& sender, const std::string& receiver);

  bool operator==(const Transaction& rhs) const;
  bool operator!=(const Transaction& rhs) const { return !(*this == rhs); }

  bool isSignatureValid() const;

  std::string m_sender;
  std::string m_receiver;
  std::string m_signature;
};

std::istream& operator>>(std::istream& lhs, Transaction& rhs);
std::ostream& operator<<(std::ostream& lhs, const Transaction& rhs);
