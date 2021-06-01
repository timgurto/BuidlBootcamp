# DigitalCash
This repository follows my implementation of the "Digital Cash" course from [mooniversity.io](https://mooniversity.io/).
The most notable difference is that I am implementing it in C++.
It is broken into a number of distinct iterations, each of which adds more functionality that is present in Bitcoin.

I use the following third-party libraries:
- Crypto++ (for keys and signing/verifying)
- Windows Rpc.h (for transaction UUIDs)

The best way to understand the functionality of each iteration is to view its tests.


## Iteration 1: StringCoin
A coin consists of a series of strings, each transferring the coin from one owner to another by name.
Coins are issued by the government, and transfers are "signed" in plaintext.

Note: in the original course, this was "PNGCoin", using pictures of written and signed notes.
Strings are functionally equivalent for the purpose of this exercise, without the irrelevant overhead of image-processing code.

[Tests](https://github.com/timgurto/DigitalCash/tree/19e9787768fc03afefb7a7a13969b451993ec6c7/DigitalCash/DigitalCash/main.cpp)
•
[Implementation](https://github.com/timgurto/DigitalCash/tree/19e9787768fc03afefb7a7a13969b451993ec6c7/DigitalCash/DigitalCash)


## Iteration 2: ECDSACoin
Cryptographic signatures are used to sign transfers, meaning one must possess the private key to spend coins owned by the corresponding public key,
and only the holder of the government's private key can issue new coins.
This prevents counterfeiting and forgery.

[Tests](https://github.com/timgurto/DigitalCash/tree/375f938aafc3558bb5a08ad37764d27491504d0b/DigitalCash/DigitalCash/main.cpp)
•
[Implementation](https://github.com/timgurto/DigitalCash/tree/375f938aafc3558bb5a08ad37764d27491504d0b/DigitalCash/DigitalCash)


## Iteration 3: BankCoin
Centralised Bank objects issue coins.
Transfers are signed and added to coins, and at any point the Bank can
observe the current state of a coin, verify the chain of transfers and signatures, and adopt the up-to-date version.
Thus, BankCoin cannot be double-spent.

[Tests](https://github.com/timgurto/DigitalCash/tree/eda74a89e9d2eadf5ff9ddeb1e3556abe103e970/DigitalCash/DigitalCash/main.cpp)
•
[Implementation](https://github.com/timgurto/DigitalCash/tree/eda74a89e9d2eadf5ff9ddeb1e3556abe103e970/DigitalCash/DigitalCash)


## Iteration 4: DivisiCoin
Each transaction consists of inputs and outputs, and purhcasing power depends on controlling (being able to sign) unspent outputs.
In this way, chains of outputs define a "coin", and there is no longer a `Coin` class.

[Tests](https://github.com/timgurto/DigitalCash/tree/0615a3f436e5424d3a6f07d3e97f079e7a283b20/DigitalCash/DigitalCash/main.cpp)
•
[Implementation](https://github.com/timgurto/DigitalCash/tree/0615a3f436e5424d3a6f07d3e97f079e7a283b20/DigitalCash/DigitalCash)
