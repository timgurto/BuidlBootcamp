#define CATCH_CONFIG_MAIN

#include "Bank.h"
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

TEST_CASE_METHOD(SampleUsers, "Issuing coins") {
  GIVEN("a bank") {
    auto bank = Bank{};

    SECTION("coins don't exist until issue()") {
      THEN("Alice has 0 coins") { CHECK(bank.checkBalance(alice) == 0); }
    }

    WHEN("it issues 1000 coins to Alice") {
      bank.issue(1000, alice);

      THEN("Alice has 1000 coins") { CHECK(bank.checkBalance(alice) == 1000); }
    }

    SECTION("the specified number of coins are issued") {
      WHEN("it issues 50 coins to Alice") {
        bank.issue(50, alice);

        THEN("Alice has 50 coins") { CHECK(bank.checkBalance(alice) == 50); }
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
      auto txID = Transaction::generateID();
      auto output0 = TxOutput{txID, 0, 100, bob};
      auto aliceToBob = Transaction{txID, {input0}, {output0}};

      WHEN("the bank handles the transaction") {
        bank.handleTransaction(aliceToBob);

        THEN("alice has 0 coins") { CHECK(bank.checkBalance(alice) == 0); }
      }
    }
  }
}

/*
auto coinbase = bank.issue(1000, alice);
txIns = {
  TxIn{txID=coinbase.id, index=0, signature=none}
}
txID = generateID();
txOuts = {
  TxOut{txID, index=0, amount=10, bob},
  TxOut{txID, index=1, amount=990, alice}
}
aliceToBob = Tx{txID, txIns, txOuts};
authAlice.signInput(aliceToBob, 0);
bank.handleTransaction(aliceToBob)
CHECK(bank.checkBalance(alice)==990);
CHECK(bank.checkBalance(bob)==10);
*/
