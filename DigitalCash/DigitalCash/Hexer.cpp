#include "Hexer.h"

#include <sstream>

using namespace std::string_literals;

std::string Hexer::convertHexToBinary(const std::string& hex) {
  if (hex.empty()) return {};

  auto iss = std::istringstream{hex};
  auto ret = std::string{};

  if (hex.size() > 2) {
    iss.get();
    iss.get();

    ret.push_back('\0');
  }

  const auto digit1 = iss.get() - '0';
  const auto digit2 = iss.get() - '0';

  auto binaryChar2 = digit1 * 16 + digit2;

  ret.push_back(binaryChar2);
  return ret;
}
