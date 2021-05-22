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
  clearCoinsFromInputs(tx);
  distributeCoinsToOutputs(tx.outputs);
}

void Bank::clearCoinsFromInputs(const Transaction& tx) {
  for (auto input = 0; input != tx.inputs.size(); ++input) {
    const auto sender = getTransactionSender(tx, input);
    m_balances[sender] = 0;
  }
}

PublicKey Bank::getTransactionSender(const Transaction& tx, Index whichInput) {
  const auto parentTxID = tx.inputs[whichInput].transactionThatOutputThis;
  const auto parentTx = m_transactions[parentTxID];
  return parentTx.outputs[0].recipient;
}

void Bank::distributeCoinsToOutputs(const Transaction::Outputs& outputs) {
  for (const auto& output : outputs) giveOutputToItsRecipient(output);
}

void Bank::giveOutputToItsRecipient(const TxOutput& output) {
  m_balances[output.recipient] = output.amount;
}
