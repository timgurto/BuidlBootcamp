#define CATCH_CONFIG_MAIN

#include "Coin.h"
#include "Transaction.h"
#include "catch.hpp"

TEST_CASE("Coin initialisation") {
  GIVEN("a new coin issued by the government to Alice") {
    auto newCoin = Coin{};
    newCoin.addTxn({"Government", "Alice", "Signed, Government"});

    WHEN("the coin's first transaction is fetched") {
      const auto txn = newCoin.getFirstTxn();

      THEN("it has the correct sender") { CHECK(txn.sender == "Government"); }
    }
  }
}

TEST_CASE("Validating a transaction") {
  CHECK(Transaction{"Alice", "Bob", "Signed, Alice"}.isSignatureValid());
  CHECK_FALSE(Transaction{"Alice", "Bob", "Signed, Bob"}.isSignatureValid());
}

// To do:
// Asking user to verify
// Validating a coin
// Serialising coins
// Deserialising coins
