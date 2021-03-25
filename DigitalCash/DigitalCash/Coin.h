#pragma once

#include <string>

class Coin {
 public:
  Coin(std::ostream &outputStream, const std::string &firstTxn)
      : m_outputStream(outputStream), m_firstTxn(firstTxn) {}

  void showFirstTxn() const { m_outputStream << m_firstTxn; }

 private:
  std::ostream &m_outputStream;
  std::string m_firstTxn;
};
