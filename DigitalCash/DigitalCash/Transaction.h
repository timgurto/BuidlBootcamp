#pragma once

#include <vector>
#include "PublicKey.h"
#include "Rpc.h"
#include "Signature.h"

using Index = unsigned;
using Currency = unsigned;
using TxID = UUID;

TxID generateTxID();
bool operator<(const TxID& lhs, const TxID& rhs);

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
  using Inputs = std::vector<TxInput>;
  using Outputs = std::vector<TxOutput>;
  Inputs inputs;
  Outputs outputs;
};
