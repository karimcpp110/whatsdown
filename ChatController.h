#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include "MessageManager.h"
#include "UserManager.h"
#include <string>
#include <vector>

class ChatController {
private:
  UserManager userManager;
  MessageManager messageManager;

public:
  // Returns true if login successful (password matches)
  bool loginUser(const std::string &id, const std::string &password);

  // Returns true if registration successful (user didn't exist)
  bool registerUser(const std::string &id, const std::string &name,
                    const std::string &password);
  bool verifyUser(const std::string &id, const std::string &password);
  void sendMessage(const std::string &senderId, const std::string &receiverId,
                   const std::string &content);
  void processMessageQueue();
  std::vector<Message> processMessagesAndReturnStructs();
  std::vector<User> getAllUsers();

  void markMessagesAsRead(const std::string &senderId,
                          const std::string &receiverId);
  void updateUserActivity(const std::string &userId);
};

#endif // CHATCONTROLLER_H
