#include "Hexer.h"

using namespace std::string_literals;

std::string Hexer::convertHexToBinary(const std::string& hex) {
  if (hex.empty()) return {};

  if (hex.size() > 2) return "\0\0"s;

  auto binaryChar = char{};
  binaryChar = (hex[0] - '0') * 16;
  binaryChar += hex[1] - '0';

  auto ret = std::string{};
  ret.push_back(binaryChar);
  return ret;
}
