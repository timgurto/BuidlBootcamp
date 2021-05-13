#include "Transfer.h"

#include "Bank.h"
#include "UserWithSigningAuthority.h"

using namespace std::string_literals;

static PublicKey getSenderFromPreviousTransfer(
    const Transfer* previousTransfer) {
  return previousTransfer->m_receiver;
}

Transfer::Transfer(const Transfer* previousTransfer, PublicKey receiver)
    : m_receiver(receiver),
      m_sender(getSenderFromPreviousTransfer(previousTransfer)) {
  if (previousTransfer)
    m_signatureOfPreviousTransfer = previousTransfer->m_signature;
  else
    m_signatureOfPreviousTransfer = Signature{"no-previous-transfer"s};

  m_signature = Signature{"unsigned"s};
}

Transfer Transfer::ReadFrom(std::istream& input) { return {input}; }

Transfer::Transfer(std::istream& input)
    : m_receiver(PublicKey::ToBeReadInto()),
      m_sender(PublicKey::ToBeReadInto()) {
  input >> *this;
}

Transfer Transfer::Issuance(const Bank& bank, PublicKey receiver) {
  return {bank, receiver};
}

Transfer::Transfer(const Bank& bank, PublicKey receiver)
    : m_receiver(receiver),
      m_sender(bank.getPublicKey()),
      m_signatureOfPreviousTransfer(Signature{"no-previous-transfer"s}),
      m_signature(Signature{"unsigned"s}) {}

bool Transfer::operator==(const Transfer& rhs) const {
  if (m_signatureOfPreviousTransfer != rhs.m_signatureOfPreviousTransfer)
    return false;
  if (m_receiver != rhs.m_receiver) return false;
  if (m_signature != rhs.m_signature) return false;

  return true;
}

std::istream& operator>>(std::istream& lhs, Transfer& rhs) {
  lhs >> rhs.m_signatureOfPreviousTransfer >> rhs.m_receiver >> rhs.m_signature;
  return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const Transfer& rhs) {
  lhs << rhs.m_signatureOfPreviousTransfer << std::endl;
  lhs << rhs.m_receiver << std::endl;
  lhs << rhs.m_signature;

  return lhs;
}

bool Transfer::isSignatureValid() const {
  if (!m_signature.exists()) return false;
  return m_sender.verifySignatureForMessage(m_signature, generateMessage());
}

Transfer::Message Transfer::generateMessage() const {
  auto oss = std::ostringstream{};
  oss << m_signatureOfPreviousTransfer << std::endl << m_receiver;
  return oss.str();
}
