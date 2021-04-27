#define CATCH_CONFIG_MAIN

#include "Coin.h"
#include "Transaction.h"
#include "UserWithSigningAuthority.h"
#include "catch.hpp"

using namespace std::string_literals;

struct SampleUsers {
 public:
  SampleUsers()
      : alice(authAlice.getWeakVersion()),
        bob(authBob.getWeakVersion()),
        charlie(authCharlie.getWeakVersion()) {}

  UserWithSigningAuthority authAlice, authBob, authCharlie;
  PublicKey alice, bob, charlie;
};

TEST_CASE_METHOD(SampleUsers, "Validating a transaction") {
  GIVEN("a transaction from Alice to Bob") {
    auto transaction = Transaction{alice, bob};

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

TEST_CASE_METHOD(SampleUsers, "Coin validity") {
  auto newCoin = Coin{};

  GIVEN("a freshly issued coin from the government to Alice") {
    newCoin = Coin::CreateByIssuingTo(alice);

    THEN("the coin is valid") { CHECK(newCoin.isValid()); }

    WHEN("Alice then passes it to Bob with her signature") {
      auto aliceToBob = Transaction{alice, bob};
      authAlice.sign(aliceToBob);
      newCoin.addTransaction(aliceToBob);

      THEN("the coin is valid") { CHECK(newCoin.isValid()); }
    }

    WHEN("Alice then passes it to Bob without signing it") {
      newCoin.addTransaction({alice, bob});
      THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
    }

    SECTION("Each sender is the previous receiver") {
      WHEN(
          "Bob (who doesn't own it) tries passing it to Charlie with his "
          "signature") {
        auto bobToCharlie = Transaction{bob, charlie};
        authBob.sign(bobToCharlie);
        newCoin.addTransaction(bobToCharlie);

        THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
      }
    }
  }

  GIVEN("a coin issued from someone other than Government") {
    newCoin.addTransaction({alice, bob});
    THEN("the coin is invalid") { CHECK_FALSE(newCoin.isValid()); }
  }

  GIVEN("an invalidly signed issuance") {
    newCoin.addTransaction({UserWithSigningAuthority::weakGovernment(), alice});

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

TEST_CASE_METHOD(SampleUsers, "Coin equality") {
  GIVEN("Two empty coins") {
    auto a = Coin{}, b = Coin{};

    THEN("they are equal") {
      CHECK(a == b);
      CHECK_FALSE(a != b);
    }

    WHEN("one has a transaction from Alice to Bob") {
      a.addTransaction({alice, bob});

      THEN("they are unequal") { CHECK(a != b); }

      AND_WHEN("the other has a transaction from Bob to Alice") {
        b.addTransaction({bob, alice});

        THEN("they are unequal") { CHECK(a != b); }
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers, "Transaction equality") {
  SECTION("Equality operators") {
    CHECK(Transaction{alice, bob} == Transaction{alice, bob});
    CHECK_FALSE(Transaction{alice, bob} != Transaction{alice, bob});
  }

  SECTION("Receivers are compared") {
    CHECK(Transaction{alice, bob} != Transaction{alice, charlie});
  }

  SECTION("Signatures are compared") {
    auto transactionSignedByAlice = Transaction{alice, bob};
    authAlice.sign(transactionSignedByAlice);

    auto transactionSignedByBob = Transaction{alice, bob};
    authBob.sign(transactionSignedByBob);

    CHECK(transactionSignedByAlice != transactionSignedByBob);
  }
}

TEST_CASE("Public key streaming") {
  GIVEN("a public key") {
    const auto originalKey = UserWithSigningAuthority{}.getWeakVersion();

    WHEN("it's converted to hex") {
      const auto asHex = originalKey.toHexString();

      THEN("the result is a hex string") {
        CHECK(asHex.size() > 0);
        CHECK((asHex[0] >= '0' && asHex[0] <= '9' ||
               asHex[0] >= 'A' && asHex[0] <= 'F'));
      }

      AND_WHEN("it's converted back") {
        auto newKey = PublicKey::ToBeReadInto();
        newKey.fromHexString(asHex);

        THEN("the new key matches the original") {
          CHECK(newKey == originalKey);
        }
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers, "Signature streaming") {
  GIVEN("a signed transaction") {
    auto signedTransaction = Transaction{alice, bob};
    authAlice.sign(signedTransaction);
    const auto originalSignature = signedTransaction.m_signature;

    WHEN("the signature is streamed out") {
      auto output = std::ostringstream{};
      output << originalSignature;
      auto serialised = output.str();

      AND_WHEN("the result is streamed back in") {
        auto input = std::istringstream{serialised};
        auto deserialisedSignature = Signature{};
        input >> deserialisedSignature;

        THEN("it matches the original signature") {
          CHECK(deserialisedSignature == originalSignature);
        }
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers, "Serialising coins") {
  SECTION("functions exist") {
    auto emptyCoin = Coin{};
    auto serialised = emptyCoin.serialise();
    Coin::CreateByDeserialising(serialised);
  }

  SECTION("Const operation") {
    const auto constCoin = Coin{};
    constCoin.serialise();
  }

  SECTION("Coins match after both conversions") {
    GIVEN("a coin") {
      auto coin = Coin{};
      const auto government = UserWithSigningAuthority::weakGovernment();

      auto THEN_theCoinSerialisesAndDeserialisesCorrectly = [&coin]() {
        THEN("the coin matches after being serialised and deserialised") {
          const auto &original = coin;
          const auto serialised = original.serialise();
          const auto deserialised = Coin::CreateByDeserialising(serialised);

          CHECK(original == deserialised);
        }
      };

      WHEN("it has a simple transaction") {
        coin.addTransaction({government, alice});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has a transaction with a different sender") {
        coin.addTransaction({bob, alice});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has a transaction with a different receiver") {
        coin.addTransaction({government, bob});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has a transaction with a different signature") {
        auto signedTransaction = Transaction{government, alice};
        authAlice.sign(signedTransaction);
        coin.addTransaction(signedTransaction);

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has two transactions") {
        coin.addTransaction({government, alice});
        coin.addTransaction({government, alice});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has three transactions") {
        coin.addTransaction({government, alice});
        coin.addTransaction({government, alice});
        coin.addTransaction({government, alice});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers,
                 "Signatures are based on the underlying transaction") {
  GIVEN("Two transactions with different senders") {
    auto fromAlice = Transaction{alice, charlie};
    auto fromBob = Transaction{bob, charlie};

    WHEN("Alice signs both") {
      authAlice.sign(fromAlice);
      authAlice.sign(fromBob);

      THEN("their signatures are different") {
        CHECK(fromAlice.m_signature != fromBob.m_signature);
      }
    }
  }
  GIVEN("Two transactions with different recipients") {
    auto toAlice = Transaction{alice, bob};
    auto toCharlie = Transaction{alice, charlie};

    WHEN("Alice signs both") {
      authAlice.sign(toAlice);
      authAlice.sign(toCharlie);

      THEN("their signatures are different") {
        CHECK(toAlice.m_signature != toCharlie.m_signature);
      }
    }
  }
}
