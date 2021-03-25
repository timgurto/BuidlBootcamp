#define CATCH_CONFIG_MAIN

#include "Coin.h"
#include "catch.hpp"

TEST_CASE("Displaying coin for verification") {
  auto outputCatcher = std::ostringstream{};

  GIVEN("a new coin issued by the government") {
    const auto newCoin =
        Coin{outputCatcher, "10 coins issued to Alice.  Signed, Government"};

    WHEN("its first transaction is shown") {
      newCoin.showFirstTxn();

      THEN("the correct string is output to the user") {
        CHECK(outputCatcher.str() ==
              "10 coins issued to Alice.  Signed, Government");
      }
    }
  }
}

// To do:
// Asking user to verify
// Validating a coin
// Serialising coins
// Deserialising coins
