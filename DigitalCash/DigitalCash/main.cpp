#define CATCH_CONFIG_MAIN

#include "Coin.h"
#include "catch.hpp"

TEST_CASE("Displaying coin for verification") {
  GIVEN("a new coin issued by the government") {
    const auto newCoin = Coin{"Issued from government to Alice"};
  }
}

// To do:
// Asking user to verify
// Defining the coin
// Validating a coin
// Serialising coins
// Deserialising coins
