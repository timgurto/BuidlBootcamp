#pragma once

// Provides functionality to convert between two string types:
// 1. Binary data; this is how Crypto++ interfaces with strings.
// 2. Hexadecimal, in which each binary character is represented by two
// hexadecimal characters.
class Hexer {
 public:
  static void convertBinaryToHex(int) {}
  static void convertHexToBinary(int) {}
};
