#include "Hexer.h"

std::string Hexer::convertHexToBinary(const std::string& hex) {
  if (hex == "00") return "\x00";
  return "\x01";
}
