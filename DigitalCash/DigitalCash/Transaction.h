#pragma once

#include <string>

struct Transaction {
  std::string sender;
  std::string receiver;
  std::string signature;
};
