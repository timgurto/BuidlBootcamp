#pragma once

#include <iostream>
#include <string>

#include "WeakUser.h"

class Transaction {
 public:
  Transaction() = default;
  Transaction(WeakUser sender, WeakUser receiver);

  bool operator==(const Transaction& rhs) const;
  bool operator!=(const Transaction& rhs) const { return !(*this == rhs); }

  bool isSignatureValid() const;

  WeakUser m_sender;
  WeakUser m_receiver;
  std::string m_signature;
};

std::istream& operator>>(std::istream& lhs, Transaction& rhs);
std::ostream& operator<<(std::ostream& lhs, const Transaction& rhs);
