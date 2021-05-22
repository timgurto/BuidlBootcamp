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
          CHECK(result.output[0].amount == 100);
        }
      }
      WHEN("the bank issues 50 coins") {
        auto result = bank.issue(50, alice);

        THEN("the resulting transaction has a 50-coin output") {
          CHECK(result.output[0].amount == 50);
        }
      }
    }
  }
}

TEST_CASE_METHOD(SampleUsers, "Transactions") {
  GIVEN("Alice is issued 100 coins") {
    auto bank = Bank{};
    auto issuance = bank.issue(100, alice);

    AND_GIVEN("a transaction of 100 coins from Alice to Bob") {
      auto input0 = TxInput{issuance.id, 0, Signature{}};
      auto txID = generateTxID();
      auto output0 = TxOutput{txID, 0, 100, bob};
      auto aliceToBob = Transaction{txID, {input0}, {output0}};
      authAlice.signInput(aliceToBob, 0);

      WHEN("the bank handles the transaction") {
        bank.handleTransaction(aliceToBob);

        THEN("alice has 0 coins") { CHECK(bank.checkBalance(alice) == 0); }
        THEN("bob has 100 coins") { CHECK(bank.checkBalance(bob) == 100); }
      }
    }

    SECTION("Multiple outputs") {
      AND_GIVEN("a transaction of 50 coins from Alice to Bob, and 50 change") {
        auto input0 = TxInput{issuance.id, 0, Signature{}};
        auto txID = generateTxID();
        auto output0 = TxOutput{txID, 0, 50, bob};
        auto output1 = TxOutput{txID, 0, 50, alice};
        auto aliceToBob = Transaction{txID, {input0}, {output0, output1}};
        authAlice.signInput(aliceToBob, 0);

        WHEN("the bank handles the transaction") {
          bank.handleTransaction(aliceToBob);

          THEN("alice has 50 coins") { CHECK(bank.checkBalance(alice) == 50); }
          THEN("bob has 50 coins") { CHECK(bank.checkBalance(bob) == 50); }
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

// Multiple senders
// Multiple receivers
// Recipient balance when amount != 100
// Issuance is unsigned
// UserWithSigningAuthority::signInput()
// Bank should check that inputs are signed
// Make sure inputs and outputs are equal
