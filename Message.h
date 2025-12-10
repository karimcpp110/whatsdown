#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

struct Message {
  std::string senderId;
  std::string receiverId;
  std::string content;
  std::string timestamp;
  bool isRead;
};

#endif // MESSAGE_H
