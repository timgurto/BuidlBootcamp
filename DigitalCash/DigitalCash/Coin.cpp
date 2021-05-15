#include "Coin.h"

#include <sstream>

#include "UserWithSigningAuthority.h"

int Coin::nextSerialToBeIssued;

bool Coin::operator==(const Coin &rhs) const {
  if (m_serial != rhs.m_serial) return false;
  return m_transfers == rhs.m_transfers;
}

Coin Coin::CreateByDeserialising(const std::string &serialisedCoin) {
  return Coin{serialisedCoin};
}

Coin::Coin(const std::string &serialisedCoin) {
  auto numTransactions = 0;

  auto iss = std::istringstream{serialisedCoin};
  iss >> m_serial >> numTransactions;

  for (auto i = 0; i != numTransactions; ++i) readAndAddTransaction(iss);
}

Coin::Coin(const Transfer &issuance) {
  appendTransfer(issuance);

  m_serial = nextSerialToBeIssued++;
}

Coin Coin::CreateByIssuing(const Transfer &issuance) { return {issuance}; }

void Coin::readAndAddTransaction(std::istringstream &serialisedCoin) {
  auto nextTransaction = Transfer::ReadFrom(serialisedCoin);
  appendTransfer(nextTransaction);
}

std::string Coin::serialise() const {
  auto oss = std::ostringstream{};

  oss << m_serial << std::endl << m_transfers.size() << std::endl;

  for (const auto &transaction : m_transfers) oss << transaction << std::endl;

  return oss.str();
}

PublicKey Coin::currentOwner() const {
  const auto lastTransfer = m_transfers.back();
  return lastTransfer.m_receiver;
}

PublicKey Coin::issuer() const {
  if (m_transfers.empty()) return PublicKey::ToBeReadInto();

  return m_transfers.front().m_sender;
}

bool Coin::allTransactionsHaveValidSignatures() const {
  for (const auto &txn : m_transfers)
    if (!txn.isSignatureValid()) return false;
  return true;
}

bool Coin::eachSpenderWasTheOwner() const {
  const auto issuee = m_transfers.front().m_receiver;

  auto ownerAtThatPoint = issuee;
  for (auto i = 1u; i < m_transfers.size(); ++i) {
    const auto &transfer = m_transfers[i];
    if (transfer.m_sender != ownerAtThatPoint) return false;
    ownerAtThatPoint = transfer.m_receiver;
  }

  return true;
}

bool Coin::isValidExtensionOf(const Coin &oldVersion) const {
  return m_transfers.size() > oldVersion.m_transfers.size();
}

const Transfer &Coin::getLastTransfer() const { return m_transfers.back(); }
