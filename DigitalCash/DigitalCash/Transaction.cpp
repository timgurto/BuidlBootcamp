#include "Transaction.h"
#include <sstream>

TxID generateTxID() {
  TxID newID;
  UuidCreate(&newID);
  return newID;
}

bool operator<(const TxID& lhs, const TxID& rhs) {
  if (lhs.Data1 != rhs.Data1) return lhs.Data1 < rhs.Data1;
  if (lhs.Data2 != rhs.Data2) return lhs.Data2 < rhs.Data2;
  if (lhs.Data3 != rhs.Data3) return lhs.Data3 < rhs.Data3;
  for (auto i = 0; i != 8; ++i) {
    if (lhs.Data4[i] != rhs.Data4[i]) return lhs.Data4[i] < rhs.Data4[i];
  }
  return false;  // UUIDs are equal
}

std::ostream& operator<<(std::ostream& lhs, const TxID& rhs) {
  auto asCString = RPC_CSTR{nullptr};
  if (UuidToStringA(&rhs, &asCString) == RPC_S_OK) {
    lhs << asCString;
    RpcStringFreeA(&asCString);
  }

  return lhs;
}

bool TxOutputID::operator<(const TxOutputID& rhs) const {
  if (transaction < rhs.transaction) return true;
  if (rhs.transaction < transaction) return false;
  return outputIndex < rhs.outputIndex;
}

std::string Transaction::getMessageForInput(Index whichInput) const {
  auto oss = std::ostringstream{};

  specifyInput(oss, whichInput);
  specifyHowOutputsWillBeSpent(oss);

  return oss.str();
}

void Transaction::specifyInput(std::ostream& os, Index whichInput) const {
  const auto& input = inputs[whichInput];
  os << input.previousOutput.transaction << std::endl;
  os << input.previousOutput.outputIndex << std::endl;
}

void Transaction::specifyHowOutputsWillBeSpent(std::ostream& os) const {
  for (const auto& output : outputs) {
    os << output.recipient << std::endl;
    os << output.amount << std::endl;
  }
}
