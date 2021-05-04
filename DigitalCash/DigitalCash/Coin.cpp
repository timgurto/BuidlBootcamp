#include "Coin.h"

#include <sstream>

#include "UserWithSigningAuthority.h"

bool Coin::operator==(const Coin &rhs) const {
  return m_transfers == rhs.m_transfers;
}

Coin Coin::CreateByDeserialising(const std::string &serialisedCoin) {
  return Coin{serialisedCoin};
}

Coin::Coin(const std::string &serialisedCoin) {
  auto numTransactions = 0;

  auto iss = std::istringstream{serialisedCoin};
  iss >> numTransactions;

  for (auto i = 0; i != numTransactions; ++i) readAndAddTransaction(iss);
}

Coin::Coin(const PublicKey &issuee) {
  auto issuanceTransaction = Transfer(nullptr, issuee);
  UserWithSigningAuthority::authGovernment().sign(issuanceTransaction);
  appendTransfer(issuanceTransaction);
}

Coin Coin::CreateByIssuingTo(const PublicKey &issuee) { return {issuee}; }

void Coin::readAndAddTransaction(std::istringstream &serialisedCoin) {
  auto nextTransaction = Transfer::ReadFrom(serialisedCoin);
  appendTransfer(nextTransaction);
}

std::string Coin::serialise() const {
  auto oss = std::ostringstream{};

  oss << m_transfers.size() << std::endl;

  for (const auto &transaction : m_transfers) oss << transaction << std::endl;

  return oss.str();
}

bool Coin::isValid() const {
  return coinWasIssuedByTheGovernment() &&
         allTransactionsHaveValidSignatures() && eachSpenderWasTheOwner();
}

bool Coin::coinWasIssuedByTheGovernment() const {
  if (m_transfers.empty()) return true;

  const auto issuer = m_transfers.front().m_sender;
  const auto expectedIssuer = UserWithSigningAuthority::weakGovernment();
  return issuer == expectedIssuer;
}

bool Coin::allTransactionsHaveValidSignatures() const {
  for (const auto &txn : m_transfers)
    if (!txn.isSignatureValid()) return false;
  return true;
}

bool Coin::eachSpenderWasTheOwner() const {
  auto ownerAtThatPoint = UserWithSigningAuthority::weakGovernment();

  for (const auto &txn : m_transfers) {
    if (txn.m_sender != ownerAtThatPoint) return false;
    ownerAtThatPoint = txn.m_receiver;
  }

  return true;
}

const Transfer *Coin::getLastTransfer() const { return &m_transfers.back(); }
