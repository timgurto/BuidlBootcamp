#pragma once

#include <iostream>
#include <string>

#include "PublicKey.h"

class Bank;

class Transfer {
 public:
  Transfer() = delete;
  Transfer(const Transfer& previousTransfer, PublicKey receiver);

  static Transfer ReadFrom(std::istream& input);
  static Transfer Issuance(const Bank& bank, PublicKey receiver);

  bool operator==(const Transfer& rhs) const;
  bool operator!=(const Transfer& rhs) const { return !(*this == rhs); }

  bool isSignatureValid() const;

  using Message = std::string;
  Message generateMessage() const;
  PublicKey m_sender;  // Fetched from previous transaction in c'tor.  Required
                       // to verify the signature.
  Signature m_signatureOfPreviousTransfer;
  PublicKey m_receiver;
  Signature m_signature;

 private:
  Transfer(std::istream& input);                   // Used by ReadFrom()
  Transfer(const Bank& bank, PublicKey receiver);  // Issuance()
};

std::istream& operator>>(std::istream& lhs, Transfer& rhs);
std::ostream& operator<<(std::ostream& lhs, const Transfer& rhs);
