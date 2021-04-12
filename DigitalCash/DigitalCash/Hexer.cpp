#include "Hexer.h"

std::string Hexer::convertHexToBinary(const std::string& hex) {
  if (hex.empty()) return {};

  auto ret = std::string{};

  if (hex[0] > '0')
    ret.push_back((hex[0] - '0') * 16);
  else
    ret.push_back(hex[1] - '0');
  return ret;
}
