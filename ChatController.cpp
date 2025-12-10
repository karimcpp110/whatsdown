#include "ChatController.h"
#include <ctime>
#include <iostream>
#include <sstream>

bool ChatController::loginUser(const std::string &id,
                               const std::string &password) {
  if (userManager.userExists(id)) {
    return userManager.verifyUser(id, password);
  }
  return false; // User not found
}

bool ChatController::registerUser(const std::string &id,
                                  const std::string &name,
                                  const std::string &password) {
  if (userManager.userExists(id)) {
    return false; // User already exists
  }
  userManager.addUser(id, name, password);
  return true;
}

bool ChatController::verifyUser(const std::string &id,
                                const std::string &password) {
  return userManager.verifyUser(id, password);
}

void ChatController::sendMessage(const std::string &senderId,
                                 const std::string &receiverId,
                                 const std::string &content) {
  if (!userManager.userExists(senderId) ||
      !userManager.userExists(receiverId)) {
    std::cout << "Sender or Receiver not found!" << std::endl;
    return;
  }

  // Get current time
  time_t now = time(0);
  tm *ltm = localtime(&now);
  char timeBuffer[6];
  sprintf(timeBuffer, "%02d:%02d", ltm->tm_hour, ltm->tm_min);
  std::string timestamp(timeBuffer);

  Message msg = {senderId, receiverId, content, timestamp, false};
  messageManager.addMessage(msg);
  std::cout << "Message sent from " << senderId << " to " << receiverId
            << " at " << timestamp << "." << std::endl;

  // Update sender activity
  updateUserActivity(senderId);
}

std::vector<Message> ChatController::processMessagesAndReturnStructs() {
  return messageManager.getAllMessages();
}

std::vector<User> ChatController::getAllUsers() {
  return userManager.getAllUsers();
}

void ChatController::markMessagesAsRead(const std::string &senderId,
                                        const std::string &receiverId) {
  messageManager.markMessagesAsRead(senderId, receiverId);
}

void ChatController::updateUserActivity(const std::string &userId) {
  userManager.updateUserActivity(userId);
}
