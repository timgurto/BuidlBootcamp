#pragma once

#include <vector>
#include "PublicKey.h"
#include "Signature.h"

using TxID = unsigned;
using Index = unsigned;
using Currency = unsigned;

struct TxInput {
  TxID transactionThatOutputThis;
  Index whichInput;
  Signature signature;
};

struct TxOutput {
  TxID owningTx;
  Index whichOutput;
  Currency amount;
  PublicKey recipient;
};

struct Transaction {
  TxID id;
  std::vector<TxInput> input;
  std::vector<TxOutput> output;

  static TxID generateID() { return {}; }
};
