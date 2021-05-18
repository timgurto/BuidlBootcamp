#pragma once

using TxID = unsigned;
using Index = unsigned;

struct TxInput {
  TxID transactionThatOutputThis;
  Index whichInput;
  Signature signature;
};

struct Transaction {
  TxID id;
};
