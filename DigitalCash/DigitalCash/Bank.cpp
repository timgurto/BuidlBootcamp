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
  const auto sender = getTransactionSender(tx);
  m_balances[sender] = 0;

  const auto receiver = tx.output[0].recipient;
  m_balances[receiver] = 100;
}

PublicKey Bank::getTransactionSender(const Transaction& tx) {
  const auto parentTxID = tx.input[0].transactionThatOutputThis;
  const auto parentTx = m_transactions[parentTxID];
  return parentTx.output[0].recipient;
}
