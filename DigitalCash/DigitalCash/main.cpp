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
    newCoin.addTransaction({"Government", "Alice", "Signed, Government"});
    THEN("the coin is valid") { CHECK(newCoin.isValid()); }

    WHEN("Alice then passes it to Bob") {
      newCoin.addTransaction({"Alice", "Bob", "Signed, Alice"});
      THEN("the coin is valid") { CHECK(newCoin.isValid()); }
    }

    WHEN("Alice then passes it to Bob with a bad signature") {
      newCoin.addTransaction({"Alice", "Bob", "Signed, Bob"});
      THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
    }
  }

  GIVEN("a coin issued from someone other than Government") {
    newCoin.addTransaction({"Alice", "Bob", "Signed, Alice"});
    THEN("the coin is invalid") { CHECK_FALSE(newCoin.isValid()); }
  }

  GIVEN("an invalidly signed issuance") {
    newCoin.addTransaction({"Government", "Alice", "Signed, Alice"});
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
      a.addTransaction({"Alice", "Bob", "Signed, Alice"});

      THEN("they are unequal") { CHECK(a != b); }

      AND_WHEN("the other has a transaction from Bob to Alice") {
        b.addTransaction({"Bob", "Alice", "Signed, Bob"});

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

TEST_CASE("Serialising coins") {
  SECTION("functions exist") {
    auto emptyCoin = Coin{};
    auto serialised = emptyCoin.serialise();
    Coin::Deserialise(serialised);
  }

  SECTION("Const operation") {
    const auto constCoin = Coin{};
    constCoin.serialise();
  }

  SECTION("Coins match after both conversions") {
    auto testSerialisationOf = [](const Coin &coin) {
      const auto &original = coin;
      const auto serialised = original.serialise();
      const auto deserialised = Coin::Deserialise(serialised);

      CHECK(original == deserialised);
    };

    GIVEN("a coin") {
      auto coin = Coin{};

      WHEN("it has a simple transaction") {
        coin.addTransaction({"Government", {}, {}});
      }

      WHEN("it has a transaction with a different sender") {
        coin.addTransaction({"Bank", {}, {}});
      }

      WHEN("it has a transaction with a different receiver") {
        coin.addTransaction({{}, "Alice", {}});
      }

      WHEN("it has a transaction with a different signature") {
        coin.addTransaction({{}, {}, "Signed, Alice"});
      }

      WHEN("it has two transactions") {
        coin.addTransaction({});
        coin.addTransaction({});
      }

      WHEN("It has two transactions, with the second being non-default") {
        coin.addTransaction({});
        coin.addTransaction({"Alice", {}, {}});
      }

      WHEN("it has three transactions") {
        coin.addTransaction({});
        coin.addTransaction({});
        coin.addTransaction({});
      }

      // THEN the coin matches after being serialised and deserialised
      testSerialisationOf(coin);
    }
  }
}

// To do:
// Serialising coins
// Deserialising coins
