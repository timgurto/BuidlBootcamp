#define CATCH_CONFIG_MAIN

#include "Coin.h"
#include "Transfer.h"
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

TEST_CASE_METHOD(SampleUsers, "Coin validity") {
  auto newCoin = Coin::CreateEmptyForTesting();

  GIVEN("a freshly issued coin from the government to Alice") {
    newCoin = Coin::CreateByIssuingTo(alice);
    const auto *lastTransfer = newCoin.getLastTransfer();

    THEN("the coin is valid") { CHECK(newCoin.isValid()); }

    WHEN("Alice then passes it to Bob with her signature") {
      auto aliceToBob = Transfer{lastTransfer, bob};
      authAlice.sign(aliceToBob);
      newCoin.addTransaction(aliceToBob);

      THEN("the coin is valid") { CHECK(newCoin.isValid()); }
    }

    WHEN("Alice then passes it to Bob without signing it") {
      newCoin.addTransaction({lastTransfer, bob});
      THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
    }

    SECTION("Each sender is the previous receiver") {
      WHEN(
          "Bob (who doesn't own it) tries passing it to Charlie with his "
          "signature") {
        auto bobToCharlie = Transfer{lastTransfer, charlie};
        authBob.sign(bobToCharlie);
        newCoin.addTransaction(bobToCharlie);

        THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
      }
    }
  }

  GIVEN("a coin issued from someone other than Government") {
    newCoin.addTransaction(Transfer{nullptr, bob});
    THEN("the coin is invalid") { CHECK_FALSE(newCoin.isValid()); }
  }

  GIVEN("an invalidly signed issuance") {
    newCoin.addTransaction({nullptr, alice});

    THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
  }

  SECTION("An empty coin doesn't crash, and is valid") {
    CHECK(newCoin.isValid());
  }

  SECTION("Coin validation is a const operation") {
    const auto constCoin = Coin::CreateEmptyForTesting();
    constCoin.isValid();
  }
}

TEST_CASE_METHOD(SampleUsers, "Coin equality") {
  GIVEN("Two empty coins") {
    auto a = Coin::CreateEmptyForTesting(), b = Coin::CreateEmptyForTesting();

    THEN("they are equal") {
      CHECK(a == b);
      CHECK_FALSE(a != b);
    }

    WHEN("one has a transaction from Alice to Bob") {
      auto firstTransfer = Transfer{nullptr, bob};
      a.addTransaction(firstTransfer);

      THEN("they are unequal") { CHECK(a != b); }

      AND_WHEN("the other has a transaction from Bob to Alice") {
        b.addTransaction({&firstTransfer, alice});

        THEN("they are unequal") { CHECK(a != b); }
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers, "Transaction equality") {
  SECTION("Equality operators") {
    CHECK(Transfer{nullptr, bob} == Transfer{nullptr, bob});
    CHECK_FALSE(Transfer{nullptr, bob} != Transfer{nullptr, bob});
  }

  SECTION("Receivers are compared") {
    CHECK(Transfer{nullptr, bob} != Transfer{nullptr, charlie});
  }

  SECTION("Signatures are compared") {
    auto transactionSignedByAlice = Transfer{nullptr, bob};
    authAlice.sign(transactionSignedByAlice);

    auto transactionSignedByBob = Transfer{nullptr, bob};
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
    auto signedTransaction = Transfer{nullptr, bob};
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
    auto emptyCoin = Coin::CreateEmptyForTesting();
    auto serialised = emptyCoin.serialise();
    Coin::CreateByDeserialising(serialised);
  }

  SECTION("Const operation") {
    const auto constCoin = Coin::CreateEmptyForTesting();
    constCoin.serialise();
  }

  SECTION("Coins match after both conversions") {
    GIVEN("a coin") {
      auto coin = Coin::CreateEmptyForTesting();
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
        coin.addTransaction({nullptr, alice});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has a transaction with a different receiver") {
        coin.addTransaction({nullptr, bob});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has a transaction with a different signature") {
        auto signedTransaction = Transfer{nullptr, alice};
        authAlice.sign(signedTransaction);
        coin.addTransaction(signedTransaction);

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has two transactions") {
        const auto firstTransfer = Transfer{nullptr, alice};
        coin.addTransaction(firstTransfer);
        coin.addTransaction({&firstTransfer, bob});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has two transactions with a different sender for the second") {
        const auto firstTransfer = Transfer{nullptr, charlie};
        coin.addTransaction(firstTransfer);
        coin.addTransaction({&firstTransfer, bob});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has three transactions") {
        coin.addTransaction({nullptr, alice});
        coin.addTransaction({nullptr, alice});
        coin.addTransaction({nullptr, alice});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers,
                 "Signatures are based on the underlying transaction") {
  GIVEN("Two transactions with different senders") {
    const auto toAlice = Transfer{nullptr, alice};
    const auto toBob = Transfer{nullptr, bob};

    auto fromAlice = Transfer{&toAlice, charlie};
    auto fromBob = Transfer{&toBob, charlie};

    WHEN("Alice signs both") {
      authAlice.sign(fromAlice);
      authAlice.sign(fromBob);

      THEN("their signatures are different") {
        CHECK(fromAlice.m_signature != fromBob.m_signature);
      }
    }
  }
  GIVEN("Two transactions with different recipients") {
    auto toAlice = Transfer{nullptr, alice};
    auto toBob = Transfer{nullptr, bob};

    WHEN("Alice signs both") {
      authAlice.sign(toAlice);
      authAlice.sign(toBob);

      THEN("their signatures are different") {
        CHECK(toAlice.m_signature != toBob.m_signature);
      }
    }
  }
}

// TODO
// Message = previous signature and next recipient (public key)
// Function to get current owner of coin
