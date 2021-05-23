#pragma once

#include <string>

using namespace std::string_literals;

class Signature {
 public:
  explicit Signature(const std::string &sig = "unsigned"s);
  bool operator==(const Signature &rhs) const;
  bool operator!=(const Signature &rhs) const;

  bool exists() const;

  const unsigned char *address() const;
  const char &firstChar() const;
  size_t size() const;

 private:
  std::string m_signature;
  friend std::istream &operator>>(std::istream &lhs, Signature &rhs);
  friend std::ostream &operator<<(std::ostream &lhs, const Signature &rhs);
};
