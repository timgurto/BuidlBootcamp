#include "Transaction.h"

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
