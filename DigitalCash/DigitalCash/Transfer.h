#pragma once

#include <iostream>
#include <string>

#include "PublicKey.h"

class Transfer {
 public:
  Transfer() = delete;
  Transfer(PublicKey sender, PublicKey receiver);

  bool operator==(const Transfer& rhs) const;
  bool operator!=(const Transfer& rhs) const { return !(*this == rhs); }

  bool isSignatureValid() const;

  static Transfer ReadFrom(std::istream& input);

  PublicKey m_sender;
  PublicKey m_receiver;
  std::string m_message;
  Signature m_signature;

 private:
  Transfer(std::istream& input);  // Used by ReadFrom()
};

std::istream& operator>>(std::istream& lhs, Transfer& rhs);
std::ostream& operator<<(std::ostream& lhs, const Transfer& rhs);
