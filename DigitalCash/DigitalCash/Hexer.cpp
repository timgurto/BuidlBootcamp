#include "Hexer.h"

#include <sstream>

using namespace std::string_literals;

std::string Hexer::convertHexToBinary(const std::string& hex) {
  if (hex.empty()) return {};

  if (hex.size() > 2) return "\0\0"s;

  auto iss = std::istringstream{hex};

  const auto digit1 = iss.get() - '0';
  const auto digit2 = iss.get() - '0';

  auto binaryChar = digit1 * 16 + digit2;

  auto ret = std::string{};
  ret.push_back(binaryChar);
  return ret;
}
