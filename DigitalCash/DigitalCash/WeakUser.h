#pragma once

// The public representation of a user.  Enough detail to send a transaction to
// this user, or to verify whether a transaction was signed by this user.
class WeakUser {
 public:
  std::string name;
};
