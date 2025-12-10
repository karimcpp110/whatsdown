#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "Message.h"
#include <vector>

class MessageManager {
private:
  std::vector<Message> messages;

public:
  MessageManager();
  void addMessage(const Message &msg);
  std::vector<Message> getAllMessages() const;
  bool isEmpty() const;
  void markMessagesAsRead(const std::string &senderId,
                          const std::string &receiverId);

private:
  void loadMessages();
  void saveMessages() const;
};

#endif // MESSAGEMANAGER_H
