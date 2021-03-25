#pragma once

#include <string>

class Coin {
 public:
  Coin(std::ostream &outputStream) : m_outputStream(outputStream) {}

  void addTxn(const std::string &txn) { m_firstTxn = txn; }
  void showFirstTxn() const { m_outputStream << m_firstTxn; }

 private:
  std::ostream &m_outputStream;
  std::string m_firstTxn;
};
