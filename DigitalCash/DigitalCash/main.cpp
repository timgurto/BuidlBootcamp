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

  SECTION("An empty coin doesn't crash, and is valid") {
    CHECK(newCoin.isValid());
  }

  SECTION("Coin validation is a const operation") {
    const auto constCoin = Coin{};
    constCoin.isValid();
  }
}

TEST_CASE("Coin equality") {
  GIVEN("Two empty coins") {
    auto a = Coin{}, b = Coin{};

    THEN("they are equal") {
      CHECK(a == b);
      CHECK_FALSE(a != b);
    }

    WHEN("one has a transaction from Alice to Bob") {
      a.addTxn({"Alice", "Bob", "Signed, Alice"});

      THEN("they are unequal") { CHECK(a != b); }

      AND_WHEN("the other has a transaction from Bob to Alice") {
        b.addTxn({"Bob", "Alice", "Signed, Bob"});

        THEN("they are unequal") { CHECK(a != b); }
      }
    }
  }
}

TEST_CASE("Transaction equality") {
  CHECK(Transaction{} == Transaction{});
  CHECK_FALSE(Transaction{} != Transaction{});

  SECTION("Receivers are compared") {
    CHECK(Transaction{{}, "Alice", {}} != Transaction{{}, "Bob", {}});
  }

  SECTION("Signatures are compared") {
    CHECK(Transaction{{}, {}, "Signed, Alice"} !=
          Transaction{{}, {}, "Signed, Bob"});
  }
}

// To do:
// Serialising coins
// Deserialising coins
