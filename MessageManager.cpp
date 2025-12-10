#include "MessageManager.h"
#include <fstream>
#include <sstream>

MessageManager::MessageManager() { loadMessages(); }

void MessageManager::addMessage(const Message &msg) {
  messages.push_back(msg);
  saveMessages();
}

std::vector<Message> MessageManager::getAllMessages() const { return messages; }

bool MessageManager::isEmpty() const { return messages.empty(); }

void MessageManager::markMessagesAsRead(const std::string &senderId,
                                        const std::string &receiverId) {
  bool changed = false;
  for (auto &msg : messages) {
    if (msg.senderId == senderId && msg.receiverId == receiverId &&
        !msg.isRead) {
      msg.isRead = true;
      changed = true;
    }
  }
  if (changed)
    saveMessages();
}

void MessageManager::loadMessages() {
  std::ifstream file("messages.txt");
  if (!file.is_open())
    return;

  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string senderId, receiverId, content, timestamp, isReadStr;
    if (std::getline(ss, senderId, '|') && std::getline(ss, receiverId, '|') &&
        std::getline(ss, content, '|') && std::getline(ss, timestamp, '|')) {

      bool isRead = false;
      if (std::getline(ss, isReadStr)) {
        isRead = (isReadStr == "1");
      }

      Message msg;
      msg.senderId = senderId;
      msg.receiverId = receiverId;
      msg.content = content;
      msg.timestamp = timestamp;
      msg.isRead = isRead;
      messages.push_back(msg);
    }
  }
}

void MessageManager::saveMessages() const {
  std::ofstream file("messages.txt");
  if (!file.is_open())
    return;

  for (const auto &msg : messages) {
    file << msg.senderId << "|" << msg.receiverId << "|" << msg.content << "|"
         << msg.timestamp << "|" << (msg.isRead ? "1" : "0") << "\n";
  }
}
