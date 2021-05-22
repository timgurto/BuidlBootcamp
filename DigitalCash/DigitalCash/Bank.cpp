#include "Bank.h"

Transaction Bank::issue(Currency amount, PublicKey recipient) {
  const auto txID = generateTxID();
  const auto output = TxOutput{txID, 0, amount, recipient};
  const auto issuance = Transaction{txID, {}, {output}};

  m_balances[recipient] = amount;
  m_transactions[txID] = issuance;
  return issuance;
}

Currency Bank::checkBalance(PublicKey account) { return m_balances[account]; }

void Bank::handleTransaction(const Transaction& tx) {
  if (!inputsMatchOutputs(tx)) return;

  clearCoinsFromInputs(tx);
  distributeCoinsToOutputs(tx.outputs);
}

bool Bank::inputsMatchOutputs(const Transaction& tx) {
  return total(tx.inputs) == total(tx.outputs);
}

Currency Bank::total(const Transaction::Inputs& inputs) {
  auto total = Currency{0};
  for (const auto& input : inputs)
    total += m_balances[currentOwnerOfInput(input)];
  return total;
}

Currency Bank::total(const Transaction::Outputs& outputs) {
  auto total = Currency{0};
  for (const auto& output : outputs) total += output.amount;
  return total;
}

void Bank::clearCoinsFromInputs(const Transaction& tx) {
  for (const auto& input : tx.inputs) {
    const auto sender = currentOwnerOfInput(input);
    m_balances[sender] = 0;
  }
}

PublicKey Bank::currentOwnerOfInput(const TxInput& input) {
  const auto parentTxID = input.transactionThatOutputThis;
  const auto parentTx = m_transactions[parentTxID];
  return parentTx.outputs[0].recipient;
}

void Bank::distributeCoinsToOutputs(const Transaction::Outputs& outputs) {
  for (const auto& output : outputs) giveOutputToItsRecipient(output);
}

void Bank::giveOutputToItsRecipient(const TxOutput& output) {
  m_balances[output.recipient] = output.amount;
}
