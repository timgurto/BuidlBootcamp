#include "Hexer.h"

std::string Hexer::convertHexToBinary(const std::string& hex) {
  if (hex.empty()) return {};

  auto binaryChar = char{};
  if (hex[0] > '0')
    binaryChar = (hex[0] - '0') * 16;
  else
    binaryChar = hex[1] - '0';

  auto ret = std::string{};
  ret.push_back(binaryChar);
  return ret;
}
