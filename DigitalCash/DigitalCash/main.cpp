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
  GIVEN("a freshly issued coin from the government to Alice") {
    auto bank = Bank{};
    bank.issueTo(alice);
    auto newCoin = *bank.coinsOwnedBy(alice).begin();
    const auto *lastTransfer = newCoin.getLastTransfer();

    THEN("the coin is valid") { CHECK(bank.isCoinValid(newCoin)); }

    WHEN("Alice then passes it to Bob with her signature") {
      auto aliceToBob = Transfer{newCoin.getLastTransfer(), bob};
      authAlice.sign(aliceToBob);
      newCoin.appendTransfer(aliceToBob);

      THEN("the coin is valid") { CHECK(bank.isCoinValid(newCoin)); }
    }

    WHEN("Alice then passes it to Bob without signing it") {
      newCoin.appendTransfer({lastTransfer, bob});
      THEN("the coin is not valid") { CHECK_FALSE(bank.isCoinValid(newCoin)); }
    }

    SECTION("Each sender is the previous receiver") {
      WHEN(
          "Bob (who doesn't own it) tries passing it to Charlie with his "
          "signature") {
        auto bobToCharlie = Transfer{lastTransfer, charlie};
        authBob.sign(bobToCharlie);
        newCoin.appendTransfer(bobToCharlie);

        THEN("the coin is not valid") {
          CHECK_FALSE(bank.isCoinValid(newCoin));
        }
      }
    }
  }

  GIVEN("an invalidly signed issuance") {
    auto bank = Bank{};
    const auto unsignedIssuance = Transfer::Issuance(bank, alice);
    auto newCoin = Coin::CreateByIssuing(unsignedIssuance);

    THEN("the coin is not valid") { CHECK_FALSE(Bank{}.isCoinValid(newCoin)); }
  }

  SECTION("An empty coin doesn't crash, and is valid") {
    auto newCoin = Coin::CreateEmptyForTesting();
    CHECK(Bank{}.isCoinValid(newCoin));
  }

  SECTION("Coin validation is a const operation") {
    const auto constCoin = Coin::CreateEmptyForTesting();
    CHECK(Bank{}.isCoinValid(constCoin));
  }
}

TEST_CASE_METHOD(SampleUsers, "Coin equality") {
  GIVEN("Two empty coins") {
    auto a = Coin::CreateEmptyForTesting(), b = Coin::CreateEmptyForTesting();

    THEN("they are equal") {
      CHECK(a == b);
      CHECK_FALSE(a != b);
    }
  }

  GIVEN("a coin issued to Alice, and a copy of it") {
    const auto issuanceTransfer = Transfer::Issuance(Bank{}, alice);
    auto a = Coin::CreateByIssuing(issuanceTransfer);
    auto b = a;

    WHEN("one has a transfer to Bob") {
      a.appendTransfer({&issuanceTransfer, bob});

      THEN("they are unequal") {
        CHECK(a != b);

        SECTION("the transfers are compared, not just counted") {
          AND_WHEN("the other has a transfer to Charlie") {
            b.appendTransfer({&issuanceTransfer, charlie});

            THEN("they are still unequal") { CHECK(a != b); }
          }
        }
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers, "Transaction equality") {
  auto bank = Bank{};

  SECTION("Equality operators") {
    CHECK(Transfer::Issuance(bank, bob) == Transfer::Issuance(bank, bob));
    CHECK_FALSE(Transfer::Issuance(bank, bob) != Transfer::Issuance(bank, bob));
  }

  SECTION("Receivers are compared") {
    CHECK(Transfer::Issuance(bank, bob) != Transfer::Issuance(bank, charlie));
  }

  SECTION("Signatures are compared") {
    auto transactionSignedByAlice = Transfer::Issuance(bank, bob);
    authAlice.sign(transactionSignedByAlice);

    auto transactionSignedByBob = Transfer::Issuance(bank, bob);
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
    auto signedTransaction = Transfer::Issuance(Bank{}, bob);
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

      auto THEN_theCoinSerialisesAndDeserialisesCorrectly = [&coin]() {
        THEN("the coin matches after being serialised and deserialised") {
          const auto &original = coin;
          const auto serialised = original.serialise();
          const auto deserialised = Coin::CreateByDeserialising(serialised);

          CHECK(original == deserialised);
        }
      };

      WHEN("it has a simple transaction") {
        coin.appendTransfer(Transfer::Issuance(Bank{}, alice));

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has a transaction with a different receiver") {
        coin.appendTransfer(Transfer::Issuance(Bank{}, alice));

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has a transaction with a different signature") {
        auto signedTransaction = Transfer::Issuance(Bank{}, alice);
        authAlice.sign(signedTransaction);
        coin.appendTransfer(signedTransaction);

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has two transactions") {
        const auto firstTransfer = Transfer::Issuance(Bank{}, alice);
        coin.appendTransfer(firstTransfer);
        coin.appendTransfer({&firstTransfer, bob});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has two transactions with a different sender for the second") {
        const auto firstTransfer = Transfer::Issuance(Bank{}, charlie);
        coin.appendTransfer(firstTransfer);
        coin.appendTransfer({&firstTransfer, bob});

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }

      WHEN("it has three transactions") {
        const auto transfer = Transfer::Issuance(Bank{}, alice);
        coin.appendTransfer(transfer);
        coin.appendTransfer(transfer);
        coin.appendTransfer(transfer);

        THEN_theCoinSerialisesAndDeserialisesCorrectly();
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers,
                 "Signatures are based on the underlying transaction") {
  auto bank = Bank{};

  GIVEN("Two transactions with different senders") {
    auto toAlice = Transfer::Issuance(bank, alice);
    auto toBob = Transfer::Issuance(bank, bob);

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
    auto toAlice = Transfer::Issuance(bank, alice);
    auto toBob = Transfer::Issuance(bank, bob);

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
    auto bank = Bank{};
    bank.issueTo(alice);
    auto coin = *bank.coinsOwnedBy(alice).begin();

    THEN("it is owned by Alice") { CHECK(coin.currentOwner() == alice); }

    WHEN("Alice gives it to Bob") {
      coin.appendTransfer({coin.getLastTransfer(), bob});

      THEN("it is owned by Bob") { CHECK(coin.currentOwner() == bob); }
    }

    SECTION("function is const") {
      GIVEN("a const reference to that coin") {
        const auto &constCoin = coin;

        THEN("its current owner can be queried") { constCoin.currentOwner(); }
      }
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
          CHECK(bank.isCoinValid(coin));
        }

        AND_WHEN("Alice appends a transfer to Bob") {
          auto alicesCopy = *alicesCoins.begin();

          auto newTransfer = Transfer{alicesCopy.getLastTransfer(), bob};
          authAlice.sign(newTransfer);
          alicesCopy.appendTransfer(newTransfer);

          AND_WHEN("the bank observes this new coin") {
            bank.observe(alicesCopy);

            THEN("Bob has a coin") {
              auto bobsCoins = bank.coinsOwnedBy(bob);
              REQUIRE(bobsCoins.size() == 1);
            }

            AND_WHEN("the bank observes this same coin again") {
              bank.observe(alicesCopy);

              THEN("Bob still has only one coin") {
                auto bobsCoins = bank.coinsOwnedBy(bob);
                REQUIRE(bobsCoins.size() == 1);
              }
            }
          }
        }

        AND_WHEN("Alice appends an unsigned transfer to Bob") {
          auto alicesCopy = *alicesCoins.begin();

          auto newTransfer = Transfer{alicesCopy.getLastTransfer(), bob};
          alicesCopy.appendTransfer(newTransfer);

          AND_WHEN("the bank observes this new coin") {
            bank.observe(alicesCopy);

            THEN("Bob still has no coins") {
              auto bobsCoins = bank.coinsOwnedBy(bob);
              REQUIRE(bobsCoins.size() == 0);
            }
          }
        }
      }

      SECTION("Multiple owners") {
        AND_WHEN("it issues a coin to Bob") {
          bank.issueTo(bob);

          THEN("Alice still has a coin") {
            auto alicesCoins = bank.coinsOwnedBy(alice);
            REQUIRE(alicesCoins.size() == 1);
          }
        }
      }

      SECTION("Multiple coins per owner") {
        AND_WHEN("it issues another coin to Alice") {
          bank.issueTo(alice);

          THEN("Alice has two coins") {
            auto alicesCoins = bank.coinsOwnedBy(alice);
            REQUIRE(alicesCoins.size() == 2);
          }
        }
      }
    }

    WHEN("it issues a coin to Bob") {
      bank.issueTo(bob);

      THEN("Alice has no coins") {
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

/*TEST_CASE_METHOD(SampleUsers, "Serial numbers are serialised") {
  GIVEN("a bank issues a coin to Alice") {
    auto bank = Bank{};
    bank.issueTo(alice);
    const auto coin = *bank.coinsOwnedBy(alice).begin();

    AND_GIVEN("a copy is made by serialising and deserialising it") {
      auto serialised = coin.serialise();
      auto copy = Coin::CreateByDeserialising(serialised);

      WHEN("the bank observes this copied coin") {
        bank.observe(copy);

        THEN("alice still owns only one coin") {
          auto alicesCoins = bank.coinsOwnedBy(alice);
          REQUIRE(alicesCoins.size() == 1);
        }
      }
    }
  }
}*/

// Transfer senders are serialised
