#ifndef USER_H
#define USER_H

#include <string>

struct User {
  std::string id;
  std::string name;
  std::string password;
  long long lastActiveTime; // Unix timestamp
};

#endif // USER_H
