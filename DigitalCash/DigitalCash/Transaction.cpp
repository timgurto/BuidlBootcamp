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

  const auto& input = inputs[whichInput];
  oss << input.previousOutput.transaction << std::endl;
  oss << input.previousOutput.outputIndex << std::endl;

  for (const auto& output : outputs) {
    oss << output.recipient << std::endl;
    oss << output.amount << std::endl;
  }

  return oss.str();
}
