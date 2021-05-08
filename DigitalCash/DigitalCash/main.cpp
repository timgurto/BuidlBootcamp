#define CATCH_CONFIG_MAIN

#include "Bank.h"
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
      newCoin.appendTransfer(aliceToBob);

      THEN("the coin is valid") { CHECK(newCoin.isValid()); }
    }

    WHEN("Alice then passes it to Bob without signing it") {
      newCoin.appendTransfer({lastTransfer, bob});
      THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
    }

    SECTION("Each sender is the previous receiver") {
      WHEN(
          "Bob (who doesn't own it) tries passing it to Charlie with his "
          "signature") {
        auto bobToCharlie = Transfer{lastTransfer, charlie};
        authBob.sign(bobToCharlie);
        newCoin.appendTransfer(bobToCharlie);

        THEN("the coin is not valid") { CHECK_FALSE(newCoin.isValid()); }
      }
    }
  }

  GIVEN("a coin issued from someone other than Government") {
    newCoin.appendTransfer(Transfer{nullptr, bob});
    THEN("the coin is invalid") { CHECK_FALSE(newCoin.isValid()); }
  }

  GIVEN("an invalidly signed issuance") {
    newCoin.appendTransfer({nullptr, alice});

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
      a.appendTransfer(firstTransfer);

      THEN("they are unequal") { CHECK(a != b); }

      AND_WHEN("the other has a transaction from Bob to Alice") {
        b.appendTransfer({&firstTransfer, alice});

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
        coin.appendTransfer({nullptr, alice});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has a transaction with a different receiver") {
        coin.appendTransfer({nullptr, bob});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has a transaction with a different signature") {
        auto signedTransaction = Transfer{nullptr, alice};
        authAlice.sign(signedTransaction);
        coin.appendTransfer(signedTransaction);

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has two transactions") {
        const auto firstTransfer = Transfer{nullptr, alice};
        coin.appendTransfer(firstTransfer);
        coin.appendTransfer({&firstTransfer, bob});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has two transactions with a different sender for the second") {
        const auto firstTransfer = Transfer{nullptr, charlie};
        coin.appendTransfer(firstTransfer);
        coin.appendTransfer({&firstTransfer, bob});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has three transactions") {
        coin.appendTransfer({nullptr, alice});
        coin.appendTransfer({nullptr, alice});
        coin.appendTransfer({nullptr, alice});

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

TEST_CASE_METHOD(SampleUsers, "Querying a coin's current owner") {
  GIVEN("a coin issued to Alice") {
    auto coin = Coin::CreateByIssuingTo(alice);

    THEN("it is owned by Alice") { CHECK(coin.currentOwner() == alice); }

    WHEN("Alice gives it to Bob") {
      coin.appendTransfer({coin.getLastTransfer(), bob});

      THEN("it is owned by Bob") { CHECK(coin.currentOwner() == bob); }
    }
  }

  SECTION("function is const") {
    GIVEN("a const coin") {
      const auto coin = Coin::CreateByIssuingTo(alice);

      THEN("its current owner can be queried") { coin.currentOwner(); }
    }
  }
}

TEST_CASE_METHOD(SampleUsers, "Bank control") {
  GIVEN("a bank") {
    auto bank = Bank{};

    THEN("Alice has no coins") {
      auto alicesCoins = bank.coinsOwnedBy(alice);
      REQUIRE(alicesCoins.empty());
    }

    WHEN("it issues a coin to Alice") {
      bank.issueTo(alice);

      THEN("Alice has a coin") {
        auto alicesCoins = bank.coinsOwnedBy(alice);
        REQUIRE(alicesCoins.size() == 1);

        AND_THEN("it is valid") {
          const auto coin = *alicesCoins.begin();
          CHECK(coin.isValid());
        }
      }

      AND_WHEN("it issues a coin to Bob") {
        bank.issueTo(bob);

        THEN("Alice still has a coin") {
          auto alicesCoins = bank.coinsOwnedBy(alice);
          REQUIRE(alicesCoins.size() == 1);
        }
      }
    }

    WHEN("it issues a coin to Bob") {
      bank.issueTo(bob);

      THEN("Alice has coins") {
        auto alicesCoins = bank.coinsOwnedBy(alice);
        REQUIRE(alicesCoins.empty());
      }
    }
  }

  SECTION("const-ness ") {
    const auto constBank = Bank{};
    constBank.coinsOwnedBy(alice);
  }
}

// Bank::issueTo(key)
// Bank::coinsOwnedBy(key): return list
// Bank::observe(coin): update official version
