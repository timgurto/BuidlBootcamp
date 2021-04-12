#include "Hexer.h"

std::string Hexer::convertHexToBinary(const std::string& hex) {
  if (hex.empty()) return {};

  auto ret = std::string{};
  ret.push_back(hex[1] - '0');
  return ret;
}
