#define CATCH_CONFIG_MAIN

#include "Coin.h"
#include "Transaction.h"
#include "catch.hpp"

TEST_CASE("Displaying coin for verification") {
  GIVEN("a new coin issued by the government to Alice") {
    auto newCoin = Coin{};
    auto txn = Transaction{"Government", "Alice", "Signed, Government"};
    newCoin.addTxn(txn);
  }
}

// To do:
// Asking user to verify
// Validating a coin
// Serialising coins
// Deserialising coins
