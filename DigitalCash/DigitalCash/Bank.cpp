#include "Bank.h"

Transaction Bank::issue(Currency amount, PublicKey recipient) {
  auto txID = Transaction::generateID();
  auto output = TxOutput{txID, 0, 0, recipient};
  auto issuance = Transaction{txID, {}, {output}};

  m_balances[recipient] = amount;
  m_transactions[txID] = issuance;
  return issuance;
}

Currency Bank::checkBalance(PublicKey account) { return m_balances[account]; }

void Bank::handleTransaction(const Transaction& tx) {
  auto sender =
      m_transactions[tx.input[0].transactionThatOutputThis].output[0].recipient;
  m_balances[sender] = 0;

  auto receiver = tx.output[0].recipient;
  m_balances[receiver] = 100;
}
