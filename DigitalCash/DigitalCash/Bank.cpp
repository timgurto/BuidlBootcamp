#include "Bank.h"

Transaction Bank::issue(Currency amount, PublicKey recipient) {
  auto txID = generateTxID();
  auto output = TxOutput{txID, 0, amount, recipient};
  auto issuance = Transaction{txID, {}, {output}};

  m_balances[recipient] = amount;
  m_transactions[txID] = issuance;
  return issuance;
}

Currency Bank::checkBalance(PublicKey account) { return m_balances[account]; }

void Bank::handleTransaction(const Transaction& tx) {
  const auto sender0 = getTransactionSender(tx, 0);
  m_balances[sender0] = 0;

  if (tx.input.size() > 1) {
    const auto sender1 = getTransactionSender(tx, 1);
    m_balances[sender1] = 0;
  }

  for (const auto& output : tx.output) distributeOutput(output);
}

void Bank::distributeOutput(const TxOutput& output) {
  m_balances[output.recipient] = output.amount;
}

PublicKey Bank::getTransactionSender(const Transaction& tx, Index whichInput) {
  const auto parentTxID = tx.input[whichInput].transactionThatOutputThis;
  const auto parentTx = m_transactions[parentTxID];
  return parentTx.output[0].recipient;
}
