#define CATCH_CONFIG_MAIN

#include "Coin.h"
#include "Transaction.h"
#include "catch.hpp"

TEST_CASE("Validating a transaction") {
  CHECK(Transaction{"Alice", "Bob", "Signed, Alice"}.isSignatureValid());
  CHECK_FALSE(Transaction{"Alice", "Bob", "Signed, Bob"}.isSignatureValid());
}

TEST_CASE("Validating a coin") {
  auto newCoin = Coin{};

  GIVEN("a freshly issued coin from the government to Alice") {
    newCoin.addTxn({"Government", "Alice", "Signed, Government"});
    THEN("the coin is valid") { CHECK(newCoin.isValid()); }

    WHEN("Alice then passes it to Bob") {
      newCoin.addTxn({"Alice", "Bob", "Signed, Alice"});
      THEN("the coin is valid") { CHECK(newCoin.isValid()); }
    }

    WHEN("Alice then passes it to Bob with a bad signature") {
      newCoin.addTxn({"Alice", "Bob", "Signed, Bob"});
      THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
    }
  }

  GIVEN("a coin issued from someone other than Government") {
    newCoin.addTxn({"Alice", "Bob", "Signed, Alice"});
    THEN("the coin is invalid") { CHECK_FALSE(newCoin.isValid()); }
  }

  GIVEN("an invalidly signed issuance") {
    newCoin.addTxn({"Government", "Alice", "Signed, Alice"});
    THEN("the coin is invalid") {
      THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
    }
  }
}

// To do:
// Serialising coins
// Deserialising coins
