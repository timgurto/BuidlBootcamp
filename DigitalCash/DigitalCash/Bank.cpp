#include "Bank.h"

Transaction Bank::issue(Currency amount, PublicKey recipient) {
  const auto txID = generateTxID();
  const auto output = TxOutput{txID, 0, amount, recipient};
  const auto issuance = Transaction{txID, {}, {output}};

  m_balances[recipient] = amount;
  m_transactions[txID] = issuance;
  return issuance;
}

Currency Bank::checkBalance(PublicKey account) const {
  const auto it = m_balances.find(account);
  if (it == m_balances.end()) return 0;
  return it->second;
}

void Bank::handleTransaction(const Transaction& tx) {
  if (!inputsMatchOutputs(tx)) return;

  removeCoinsFromInputs(tx);
  addCoinsToOutputs(tx.outputs);

  m_transactions[tx.id] = tx;
}

bool Bank::inputsMatchOutputs(const Transaction& tx) const {
  return total(tx.inputs) == total(tx.outputs);
}

Currency Bank::total(const Transaction::Inputs& inputs) const {
  auto total = Currency{0};
  for (const auto& input : inputs) {
    const auto inputAmount = correspondingUTXO(input).amount;
    total += inputAmount;
  }
  return total;
}

Currency Bank::total(const Transaction::Outputs& outputs) {
  auto total = Currency{0};
  for (const auto& output : outputs) total += output.amount;
  return total;
}

void Bank::removeCoinsFromInputs(const Transaction& tx) {
  for (const auto& input : tx.inputs) {
    const auto& asUTXO = correspondingUTXO(input);
    const auto sender = asUTXO.recipient;
    const auto amount = asUTXO.amount;
    m_balances[sender] -= amount;
  }
}

const TxOutput& Bank::correspondingUTXO(const TxInput& input) const {
  const auto parentTxID = input.transactionThatOutputThis;
  const auto parentTx = m_transactions.find(parentTxID);
  return parentTx->second.outputs[input.whichOutputWasThis];
}

void Bank::addCoinsToOutputs(const Transaction::Outputs& outputs) {
  for (const auto& output : outputs) giveOutputToItsRecipient(output);
}

void Bank::giveOutputToItsRecipient(const TxOutput& output) {
  m_balances[output.recipient] += output.amount;
}
