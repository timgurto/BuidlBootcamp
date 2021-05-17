#define CATCH_CONFIG_MAIN

#include "Bank.h"
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

TEST_CASE_METHOD(SampleUsers, "Standard use case") {
  GIVEN("the bank issues 1000 coins to Alice") {
    auto bank = Bank{};
    bank.issue(1000, alice);

    THEN("Alice has 1000 coins") { CHECK(bank.checkBalance(alice) == 1000); }
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
CHECK(bank.checkBalance(alice)==990);
CHECK(bank.checkBalance(bob)==10);

 Transaction
- input[]
- output[]

 TxIn
- transaction ID
- index within transaction inputs
- signature

 TxOut
- transaction ID
- index within transaction outputs
- amount
- public key
*/
