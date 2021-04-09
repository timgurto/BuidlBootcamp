#define CATCH_CONFIG_RUNNER

#include "Coin.h"
#include "Transaction.h"
#include "UserWithSigningAuthority.h"
#include "catch.hpp"

int main(int argc, char *argv[]) {
  int result = Catch::Session().run(argc, argv);
  return result;
}

struct AliceAndBob {
 public:
  AliceAndBob()
      : weakAlice(authAlice.getWeakVersion()),
        weakBob(authBob.getWeakVersion()) {}

  UserWithSigningAuthority authAlice, authBob;
  PublicKey weakAlice, weakBob;
};

TEST_CASE_METHOD(AliceAndBob, "Validating a transaction") {
  GIVEN("a transaction from Alice to Bob") {
    auto transaction = Transaction{weakAlice, weakBob};

    WHEN("Alice signs it") {
      authAlice.sign(transaction);

      THEN("it is valid") { CHECK(transaction.isSignatureValid()); }
    }

    WHEN("Someone other than Alice signs it") {
      authBob.sign(transaction);
      THEN("it is not valid") { CHECK_FALSE(transaction.isSignatureValid()); }
    }
  }
}

TEST_CASE_METHOD(AliceAndBob, "Coin validity") {
  auto newCoin = Coin{};

  GIVEN("a freshly issued coin from the government to Alice") {
    auto issuance =
        Transaction{UserWithSigningAuthority::weakGovernment(), weakAlice};
    UserWithSigningAuthority::authGovernment().sign(issuance);
    newCoin.addTransaction(issuance);

    THEN("the coin is valid") { CHECK(newCoin.isValid()); }

    WHEN("Alice then passes it to Bob with her signature") {
      auto aliceToBob = Transaction{weakAlice, weakBob};
      authAlice.sign(aliceToBob);
      newCoin.addTransaction(aliceToBob);

      THEN("the coin is valid") { CHECK(newCoin.isValid()); }
    }

    WHEN("Alice then passes it to Bob without signing it") {
      newCoin.addTransaction({weakAlice, weakBob});
      THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
    }
  }

  GIVEN("a coin issued from someone other than Government") {
    newCoin.addTransaction({weakAlice, weakBob});
    THEN("the coin is invalid") { CHECK_FALSE(newCoin.isValid()); }
  }

  GIVEN("an invalidly signed issuance") {
    newCoin.addTransaction(
        {UserWithSigningAuthority::weakGovernment(), weakAlice});
    THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
  }

  SECTION("An empty coin doesn't crash, and is valid") {
    CHECK(newCoin.isValid());
  }

  SECTION("Coin validation is a const operation") {
    const auto constCoin = Coin{};
    constCoin.isValid();
  }
}

TEST_CASE_METHOD(AliceAndBob, "Coin equality") {
  GIVEN("Two empty coins") {
    auto a = Coin{}, b = Coin{};

    THEN("they are equal") {
      CHECK(a == b);
      CHECK_FALSE(a != b);
    }

    WHEN("one has a transaction from Alice to Bob") {
      a.addTransaction({weakAlice, weakBob});

      THEN("they are unequal") { CHECK(a != b); }

      AND_WHEN("the other has a transaction from Bob to Alice") {
        b.addTransaction({weakBob, weakAlice});

        THEN("they are unequal") { CHECK(a != b); }
      }
    }
  }
}

TEST_CASE_METHOD(AliceAndBob, "Transaction equality") {
  CHECK(Transaction{} == Transaction{});
  CHECK_FALSE(Transaction{} != Transaction{});

  SECTION("Receivers are compared") {
    CHECK(Transaction{{}, weakAlice} != Transaction{{}, weakBob});
  }

  SECTION("Signatures are compared") {
    auto transactionSignedByAlice = Transaction{};
    authAlice.sign(transactionSignedByAlice);

    auto transactionSignedByBob = Transaction{};
    authBob.sign(transactionSignedByBob);

    CHECK(transactionSignedByAlice != transactionSignedByBob);
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
      const auto authAlice = UserWithSigningAuthority{};
      const auto weakAlice = authAlice.getWeakVersion();

      auto coin = Coin{};

      WHEN("it has a simple transaction") {
        coin.addTransaction({UserWithSigningAuthority::weakGovernment(), {}});
      }

      WHEN("it has a transaction with a different sender") {
        const auto weakBank = UserWithSigningAuthority{}.getWeakVersion();
        coin.addTransaction({weakBank, {}});
      }

      WHEN("it has a transaction with a different receiver") {
        coin.addTransaction({{}, weakAlice});
      }

      WHEN("it has a transaction with a different signature") {
        auto signedTransaction = Transaction{};
        authAlice.sign(signedTransaction);
        coin.addTransaction(signedTransaction);
      }

      WHEN("it has two transactions") {
        coin.addTransaction({});
        coin.addTransaction({});
      }

      WHEN("It has two transactions, with the second being non-default") {
        coin.addTransaction({});
        coin.addTransaction({weakAlice, {}});
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

TEST_CASE_METHOD(AliceAndBob,
                 "Signatures are based on the underlying transaction") {
  GIVEN("Two transactions with different senders") {
    auto fromAlice = Transaction{weakAlice, {}};
    auto fromBob = Transaction{weakBob, {}};

    WHEN("Alice signs both") {
      authAlice.sign(fromAlice);
      authAlice.sign(fromBob);

      THEN("their signatures are different") {
        CHECK(fromAlice.m_signature != fromBob.m_signature);
      }
    }
  }
  GIVEN("Two transactions with different recipients") {
    auto toAlice = Transaction{{}, weakAlice};
    auto toBob = Transaction{{}, weakBob};

    WHEN("Alice signs both") {
      authAlice.sign(toAlice);
      authAlice.sign(toBob);

      THEN("their signatures are different") {
        CHECK(toAlice.m_signature != toBob.m_signature);
      }
    }
  }
}

// TODO: drop support for empty transactions/keys
