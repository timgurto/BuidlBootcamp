#pragma once

using TxID = unsigned;
using Index = unsigned;

struct TxInput {
  TxID transactionThatOutputThis;
  Index whichInput;
  Signature signature;
};

struct TxOutput {};

struct Transaction {
  TxID id;

  static void generateID() {}
};
