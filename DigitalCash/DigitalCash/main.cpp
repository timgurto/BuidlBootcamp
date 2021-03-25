#define CATCH_CONFIG_MAIN

#include "Coin.h"
#include "catch.hpp"

TEST_CASE("Displaying coin for verification") {
  auto outputCatcher = std::ostringstream{};
  auto newCoin = Coin{outputCatcher};

  GIVEN("a new coin issued by the government to Alice") {
    newCoin.addTxn("10 coins issued to Alice.  Signed, Government");

    WHEN("its first transaction is shown") {
      newCoin.showFirstTxn();

      THEN("the correct string is output to the user") {
        CHECK(outputCatcher.str() ==
              "10 coins issued to Alice.  Signed, Government");
      }
    }
  }

  SECTION("Different transaction text also works") {
    GIVEN("a new coin issued by the government to Bob") {
      newCoin.addTxn("10 coins issued to Bob.  Signed, Government");

      WHEN("its first transaction is shown") {
        newCoin.showFirstTxn();

        THEN("the correct string is output to the user") {
          CHECK(outputCatcher.str() ==
                "10 coins issued to Bob.  Signed, Government");
        }
      }
    }
  }
}

// To do:
// Asking user to verify
// Validating a coin
// Serialising coins
// Deserialising coins
