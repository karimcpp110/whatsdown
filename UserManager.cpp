#include "UserManager.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

UserManager::UserManager() { loadUsers(); }

void UserManager::addUser(const std::string &id, const std::string &name,
                          const std::string &password) {
  if (!userExists(id)) {
    users[id] = {id, name, password, std::time(nullptr)};
    saveUsers();
  }
}

User *UserManager::getUser(const std::string &id) {
  if (users.find(id) != users.end()) {
    return &users[id];
  }
  return nullptr;
}

bool UserManager::userExists(const std::string &id) const {
  return users.find(id) != users.end();
}

bool UserManager::verifyUser(const std::string &id,
                             const std::string &password) const {
  auto it = users.find(id);
  if (it != users.end()) {
    return it->second.password == password;
  }
  return false;
}

void UserManager::updateUserActivity(const std::string &id) {
  if (users.find(id) != users.end()) {
    users[id].lastActiveTime = std::time(nullptr);
  }
}

std::vector<User> UserManager::getAllUsers() const {
  std::vector<User> allUsers;
  for (const auto &pair : users) {
    allUsers.push_back(pair.second);
  }
  return allUsers;
}

void UserManager::loadUsers() {
  std::ifstream file("users.txt");
  if (!file.is_open()) {
    std::cout << "Warning: Could not open users.txt for reading." << std::endl;
    return;
  }
  std::cout << "Loading users from users.txt..." << std::endl;

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty())
      continue;
    std::stringstream ss(line);
    std::string id, name, token3, token4;

    if (std::getline(ss, id, '|') && std::getline(ss, name, '|')) {
      // Read remaining tokens
      std::getline(ss, token3, '|');
      std::getline(ss, token4); // Read rest of line

      std::string password = "0000";
      long long lastActive = 0;

      if (!token4.empty()) {
        // New Format: ID|Name|Password|Timestamp
        password = token3;
        try {
          lastActive = std::stoll(token4);
        } catch (...) {
        }
      } else {
        // Old Format: ID|Name|Timestamp (token3 captures timestamp)
        try {
          lastActive = std::stoll(token3);
        } catch (...) {
        }
      }

      users[id] = {id, name, password, lastActive};
    }
  }
  std::cout << "Loaded " << users.size() << " users." << std::endl;
}

void UserManager::saveUsers() const {
  std::ofstream file("users.txt");
  if (!file.is_open()) {
    std::cout << "Error: Could not open users.txt for writing." << std::endl;
    return;
  }

  for (const auto &pair : users) {
    file << pair.first << "|" << pair.second.name << "|" << pair.second.password
         << "|" << pair.second.lastActiveTime << "\n";
  }
  std::cout << "Saved " << users.size() << " users to users.txt." << std::endl;
}
