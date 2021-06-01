#define CATCH_CONFIG_MAIN

#include "Bank.h"
#include "Transaction.h"
#include "UserWithSigningAuthority.h"

// These are defined by Windows headers, but Catch requires its own definitions
#undef min
#undef max
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

TEST_CASE("Public-key streaming") {
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

TEST_CASE_METHOD(SampleUsers, "Public-key comparisons") {
  CHECK((alice < bob) != (bob < alice));
}

TEST_CASE_METHOD(SampleUsers, "Issuing coins") {
  GIVEN("a bank") {
    auto bank = Bank{};

    SECTION("coins don't exist until issue()") {
      THEN("Alice has 0 coins") { CHECK(bank.checkBalance(alice) == 0); }
    }

    WHEN("it issues 1000 coins to Alice") {
      bank.issue(1000, alice);

      THEN("Alice has 1000 coins") { CHECK(bank.checkBalance(alice) == 1000); }
      THEN("Bob has 0 coins") { CHECK(bank.checkBalance(bob) == 0); }
    }

    SECTION("the specified number of coins are issued") {
      WHEN("it issues 50 coins to Alice") {
        bank.issue(50, alice);

        THEN("Alice has 50 coins") { CHECK(bank.checkBalance(alice) == 50); }
      }
    }

    SECTION("Return value") {
      WHEN("the bank issues 100 coins") {
        auto result = bank.issue(100, alice);

        THEN("the resulting transaction has a 100-coin output") {
          CHECK(result.outputs[0].amount == 100);
        }
      }
      WHEN("the bank issues 50 coins") {
        auto result = bank.issue(50, alice);

        THEN("the resulting transaction has a 50-coin output") {
          CHECK(result.outputs[0].amount == 50);
        }
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers, "Transactions") {
  GIVEN("Alice is issued 100 coins") {
    auto bank = Bank{};
    auto issuanceToAlice = bank.issue(100, alice);

    AND_GIVEN("a transaction of 100 coins from Alice to Bob") {
      auto input0 = TxInput{issuanceToAlice.id, 0, Signature{}};
      auto txID = generateTxID();
      auto output0 = TxOutput{txID, 0, 100, bob};
      auto aliceToBob = Transaction{txID, {input0}, {output0}};
      authAlice.signInput(aliceToBob, 0);

      WHEN("the bank handles the transaction") {
        bank.handleTransaction(aliceToBob);

        THEN("Alice has 0 coins") { CHECK(bank.checkBalance(alice) == 0); }
        THEN("Bob has 100 coins") { CHECK(bank.checkBalance(bob) == 100); }
      }
    }

    SECTION("Multiple outputs") {
      AND_GIVEN("a transaction of 50 coins from Alice to Bob, and 50 change") {
        auto input0 = TxInput{issuanceToAlice.id, 0, Signature{}};
        auto txID = generateTxID();
        auto output0 = TxOutput{txID, 0, 50, bob};
        auto output1 = TxOutput{txID, 0, 50, alice};
        auto aliceToBob = Transaction{txID, {input0}, {output0, output1}};
        authAlice.signInput(aliceToBob, 0);

        WHEN("the bank handles the transaction") {
          bank.handleTransaction(aliceToBob);

          THEN("Alice has 50 coins") { CHECK(bank.checkBalance(alice) == 50); }
          THEN("Bob has 50 coins") { CHECK(bank.checkBalance(bob) == 50); }
        }
      }
      AND_GIVEN("a transaction of 10 coins from Alice to Bob, and 90 change") {
        auto input0 = TxInput{issuanceToAlice.id, 0, Signature{}};
        auto txID = generateTxID();
        auto output0 = TxOutput{txID, 0, 10, bob};
        auto output1 = TxOutput{txID, 0, 90, alice};
        auto aliceToBob = Transaction{txID, {input0}, {output0, output1}};
        authAlice.signInput(aliceToBob, 0);

        WHEN("the bank handles the transaction") {
          bank.handleTransaction(aliceToBob);

          THEN("Alice has 50 coins") { CHECK(bank.checkBalance(alice) == 90); }
          THEN("Bob has 50 coins") { CHECK(bank.checkBalance(bob) == 10); }
        }
      }
    }

    SECTION("Multiple inputs") {
      AND_GIVEN("Bob is issued 100 coins") {
        auto issuanceToBob = bank.issue(100, bob);

        AND_GIVEN("Alice and Bob give all 200 coins to Charlie") {
          auto input0 = TxInput{issuanceToAlice.id, 0, Signature{}};
          auto input1 = TxInput{issuanceToBob.id, 0, Signature{}};
          auto txID = generateTxID();
          auto output0 = TxOutput{txID, 0, 200, charlie};
          auto allToCharlie = Transaction{txID, {input0, input1}, {output0}};
          authAlice.signInput(allToCharlie, 0);
          authBob.signInput(allToCharlie, 1);
          bank.handleTransaction(allToCharlie);

          THEN("Bob has 0 coins") { CHECK(bank.checkBalance(bob) == 0); }
        }
      }
    }

    SECTION("Unequal inputs/outputs: transaction is ignored") {
      AND_GIVEN("a transaction of 50 coins from Alice to Bob") {
        auto input0 = TxInput{issuanceToAlice.id, 0, Signature{}};
        auto txID = generateTxID();
        auto output0 = TxOutput{txID, 0, 50, bob};
        auto aliceToBob = Transaction{txID, {input0}, {output0}};
        authAlice.signInput(aliceToBob, 0);

        WHEN("the bank handles the transaction") {
          bank.handleTransaction(aliceToBob);

          THEN("Alice still has 100 coins") {
            CHECK(bank.checkBalance(alice) == 100);
          }
        }
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers, "Using an output[1] as input") {
  GIVEN("Alice has two coins") {
    auto bank = Bank{};
    auto issuanceToAlice = bank.issue(2, alice);

    AND_GIVEN("Bob receives one coin as output[1]") {
      auto input0 = TxInput{issuanceToAlice.id, 0, Signature{}};
      auto txID = generateTxID();
      auto output0 = TxOutput{txID, 0, 1, alice};
      auto output1 = TxOutput{txID, 1, 1, bob};
      auto aliceToBob = Transaction{txID, {input0}, {output0, output1}};
      authAlice.signInput(aliceToBob, 0);
      bank.handleTransaction(aliceToBob);

      WHEN("Bob then spends his coin (what was output[1])") {
        auto input0 = TxInput{aliceToBob.id, 1, Signature{}};
        auto txID = generateTxID();
        auto output0 = TxOutput{txID, 0, 1, charlie};
        auto bobToCharlie = Transaction{txID, {input0}, {output0}};
        authBob.signInput(bobToCharlie, 0);
        bank.handleTransaction(bobToCharlie);

        THEN("he has no coins") { CHECK(bank.checkBalance(bob) == 0); }
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers,
                 "Balance includes all UTXOs, not just the most recent") {
  GIVEN("Alice has two coins") {
    auto bank = Bank{};
    auto issuanceToAlice = bank.issue(2, alice);

    WHEN("she gives one to Bob") {
      auto input0 = TxInput{issuanceToAlice.id, 0, Signature{}};
      auto txID = generateTxID();
      auto output0 = TxOutput{txID, 0, 1, bob};
      auto output1 = TxOutput{txID, 1, 1, alice};  // Change
      auto aliceToBob1 = Transaction{txID, {input0}, {output0, output1}};
      authAlice.signInput(aliceToBob1, 0);
      bank.handleTransaction(aliceToBob1);

      AND_WHEN("she gives another to Bob") {
        auto input0 = TxInput{aliceToBob1.id, 1, Signature{}};
        auto txID = generateTxID();
        auto output1 = TxOutput{txID, 0, 1, bob};
        auto aliceToBob2 = Transaction{txID, {input0}, {output0}};
        authAlice.signInput(aliceToBob2, 0);
        bank.handleTransaction(aliceToBob2);

        THEN("Bob has two coins") { CHECK(bank.checkBalance(bob) == 2); }

        SECTION("Spending one doesn't remove the other") {
          AND_WHEN("Bob spends one of them") {
            auto input0 = TxInput{aliceToBob2.id, 0, Signature{}};
            auto txID = generateTxID();
            auto output0 = TxOutput{txID, 0, 1, alice};
            auto bobToAlice = Transaction{txID, {input0}, {output0}};
            authBob.signInput(bobToAlice, 0);
            bank.handleTransaction(bobToAlice);

            THEN("he still has one coin") {
              CHECK(bank.checkBalance(bob) == 1);
            }
          }
        }
      }
    }
  }
}

TEST_CASE("TxIDs are unique") {
  WHEN("Two TxIDs are generated") {
    auto id1 = generateTxID();
    auto id2 = generateTxID();

    THEN("they are different") { CHECK(id1 != id2); }
  }
}

TEST_CASE_METHOD(SampleUsers, "Signatures on inputs") {
  GIVEN("Alice has a coin") {
    auto bank = Bank{};
    auto issuanceToAlice = bank.issue(1, alice);

    AND_GIVEN("an unsigned transaction giving the coin to Bob") {
      auto input0 = TxInput{issuanceToAlice.id, 0, Signature{}};
      auto txID = generateTxID();
      auto output0 = TxOutput{txID, 0, 1, bob};
      auto aliceToBob = Transaction{txID, {input0}, {output0}};

      SECTION("Unsigned") {
        WHEN("the bank observes it") {
          bank.handleTransaction(aliceToBob);

          THEN("the transaction failed (Alice still has the coin)") {
            CHECK(bank.checkBalance(alice) == 1);
          }
        }
      }

      SECTION("Signed by the wrong person") {
        WHEN("Charlie signs it") {
          authCharlie.signInput(aliceToBob, 0);

          AND_WHEN("the bank observes it") {
            bank.handleTransaction(aliceToBob);

            THEN("the transaction failed (Alice still has the coin)") {
              CHECK(bank.checkBalance(alice) == 1);
            }
          }
        }
      }
    }
  }
  SECTION("All inputs are signed, not just first") {
    GIVEN("Alice and Bob each have a coin") {
      auto bank = Bank{};
      auto issuanceToAlice = bank.issue(1, alice);
      auto issuanceToBob = bank.issue(1, bob);

      AND_GIVEN("a transaction from Alice and Bob to Charlie") {
        auto input0 = TxInput{issuanceToAlice.id, 0, Signature{}};
        auto input1 = TxInput{issuanceToBob.id, 0, Signature{}};
        auto txID = generateTxID();
        auto output0 = TxOutput{txID, 0, 2, charlie};
        auto bothToCharlie = Transaction{txID, {input0, input1}, {output0}};

        AND_GIVEN("only Alice's input is signed") {
          authAlice.signInput(bothToCharlie, 0);

          SECTION("Unsigned") {
            WHEN("the bank observes the transaction") {
              bank.handleTransaction(bothToCharlie);

              THEN("the transaction failed (Charlie still has no coins)") {
                CHECK(bank.checkBalance(charlie) == 0);
              }
            }
          }

          SECTION("Signed by the wrong person") {
            WHEN("Bob's input is signed by someone other than Bob") {
              const auto stranger = UserWithSigningAuthority{};
              stranger.signInput(bothToCharlie, 1);

              AND_WHEN("the bank observes the transaction") {
                bank.handleTransaction(bothToCharlie);

                THEN("the transaction fails (Charlie still has no coins)") {
                  CHECK(bank.checkBalance(charlie) == 0);
                }
              }
            }
          }
        }
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers, "Double spending is prevented") {
  GIVEN("Alice has plenty of coins") {
    auto bank = Bank{};
    bank.issue(10, alice);

    AND_GIVEN("Alice has an output of 1 coin") {
      const auto issuance = bank.issue(1, alice);

      AND_GIVEN("she has sent it to Bob") {
        const auto input = TxInput{issuance.id, 0, Signature{}};
        const auto txID = generateTxID();
        const auto output = TxOutput{txID, 0, 1, bob};
        auto toBob = Transaction{txID, {input}, {output}};
        authAlice.signInput(toBob, 0);
        bank.handleTransaction(toBob);

        AND_WHEN("she tries also sending it to Charlie") {
          const auto txID = generateTxID();
          const auto output = TxOutput{txID, 0, 1, charlie};
          auto toCharlie = Transaction{txID, {input}, {output}};
          authAlice.signInput(toCharlie, 0);
          bank.handleTransaction(toCharlie);

          THEN("the transaction fails (Charlie still has no coins)") {
            CHECK(bank.checkBalance(charlie) == 0);
          }
        }
      }
    }
  }
}
