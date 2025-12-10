#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "User.h"
#include <unordered_map>
#include <vector>

class UserManager {
private:
  std::unordered_map<std::string, User> users;

public:
  UserManager();
  void addUser(const std::string &id, const std::string &name,
               const std::string &password);
  User *getUser(const std::string &id);
  bool userExists(const std::string &id) const;
  bool verifyUser(const std::string &id, const std::string &password) const;
  void updateUserActivity(const std::string &id);
  std::vector<User> getAllUsers() const;

private:
  void loadUsers();
  void saveUsers() const;
};

#endif // USERMANAGER_H
