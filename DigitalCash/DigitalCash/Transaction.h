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

struct TxOutputID {
  TxID transaction;
  Index outputIndex;
  bool operator<(const TxOutputID& rhs) const;
};

struct TxInput {
  TxOutputID previousOutput;
  Signature signature;
};

struct TxOutput {
  TxOutputID id;
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
