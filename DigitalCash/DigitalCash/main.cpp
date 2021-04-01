#define CATCH_CONFIG_RUNNER

#include "Coin.h"
#include "Transaction.h"
#include "catch.hpp"

int main(int argc, char *argv[]) {
  int result = Catch::Session().run(argc, argv);
  return result;
}

struct User {
  std::string name;
  std::string signature;
};
const auto government = User{"Government", "Signed, Government"};
const auto alice = User{"Alice", "Signed, Alice"};
const auto bob = User{"Bob", "Signed, Bob"};

TEST_CASE("Validating a transaction") {
  CHECK(Transaction{alice.name, bob.name, alice.signature}.isSignatureValid());
  CHECK_FALSE(
      Transaction{alice.name, bob.name, bob.signature}.isSignatureValid());
}

TEST_CASE("Validating a coin") {
  auto newCoin = Coin{};

  GIVEN("a freshly issued coin from the government to Alice") {
    newCoin.addTransaction({government.name, alice.name, government.signature});
    THEN("the coin is valid") { CHECK(newCoin.isValid()); }

    WHEN("Alice then passes it to Bob") {
      newCoin.addTransaction({alice.name, bob.name, alice.signature});
      THEN("the coin is valid") { CHECK(newCoin.isValid()); }
    }

    WHEN("Alice then passes it to Bob with a bad signature") {
      newCoin.addTransaction({alice.name, bob.name, bob.signature});
      THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
    }
  }

  GIVEN("a coin issued from someone other than Government") {
    newCoin.addTransaction({alice.name, bob.name, alice.signature});
    THEN("the coin is invalid") { CHECK_FALSE(newCoin.isValid()); }
  }

  GIVEN("an invalidly signed issuance") {
    newCoin.addTransaction({government.name, alice.name, alice.signature});
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
      a.addTransaction({alice.name, bob.name, alice.signature});

      THEN("they are unequal") { CHECK(a != b); }

      AND_WHEN("the other has a transaction from Bob to Alice") {
        b.addTransaction({bob.name, alice.name, bob.signature});

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
    CHECK(Transaction{{}, {}, alice.signature} !=
          Transaction{{}, {}, bob.signature});
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
        coin.addTransaction({government.name, {}, {}});
      }

      WHEN("it has a transaction with a different sender") {
        coin.addTransaction({"Bank", {}, {}});
      }

      WHEN("it has a transaction with a different receiver") {
        coin.addTransaction({{}, alice.name, {}});
      }

      WHEN("it has a transaction with a different signature") {
        coin.addTransaction({{}, {}, alice.signature});
      }

      WHEN("it has two transactions") {
        coin.addTransaction({});
        coin.addTransaction({});
      }

      WHEN("It has two transactions, with the second being non-default") {
        coin.addTransaction({});
        coin.addTransaction({alice.name, {}, {}});
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
