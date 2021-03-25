#pragma once

#include <string>

class Coin {
 public:
  Coin(std::ostream &outputStream, const std::string &)
      : m_outputStream(outputStream) {}

  void showFirstTxn() const {
    m_outputStream << "10 coins issued to Alice.  Signed, Government";
  }

 private:
  std::ostream &m_outputStream;
};
