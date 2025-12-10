#include "ChatController.h"
#include "httplib.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace httplib;

// Helper to read file content
string readFile(const string &path) {
  ifstream file(path);
  if (!file.is_open())
    return "";
  stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

// Helper to add CORS headers
void addCORS(Response &res) {
  res.set_header("Access-Control-Allow-Origin", "*");
  res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
  ChatController server;
  Server svr;

  // Global OPTIONS handler for Preflight requests
  svr.Options(".*", [](const Request &req, Response &res) {
    addCORS(res);
    res.set_content("", "text/plain");
  });

  // Serve static files
  svr.Get("/", [](const Request &req, Response &res) {
    string content = readFile("index.html");
    res.set_content(content, "text/html");
  });

  svr.Get("/style.css", [](const Request &req, Response &res) {
    string content = readFile("style.css");
    res.set_content(content, "text/css");
  });

  svr.Get("/script.js", [](const Request &req, Response &res) {
    string content = readFile("script.js");
    res.set_content(content, "application/javascript");
  });

  // API: Login
  // API: Login
  svr.Post("/login", [&](const Request &req, Response &res) {
    addCORS(res);
    if (req.has_param("id") && req.has_param("password")) {
      string id = req.get_param_value("id");
      string password = req.get_param_value("password");

      if (server.loginUser(id, password)) {
        server.updateUserActivity(id);
        // Return user name for frontend convenience
        User *user = server.getUser(id);
        string name = user ? user->name : "User";
        res.set_content(name, "text/plain");
      } else {
        res.status = 401; // Unauthorized
        res.set_content("Invalid Credentials", "text/plain");
      }
    } else {
      res.status = 400;
      res.set_content("Missing parameters", "text/plain");
    }
  });

  // API: Register
  svr.Post("/register", [&](const Request &req, Response &res) {
    addCORS(res);
    if (req.has_param("id") && req.has_param("name") &&
        req.has_param("password")) {
      string id = req.get_param_value("id");
      string name = req.get_param_value("name");
      string password = req.get_param_value("password");

      if (server.registerUser(id, name, password)) {
        server.updateUserActivity(id);
        res.set_content("Registered", "text/plain");
      } else {
        res.status = 409; // Conflict
        res.set_content("User ID already exists", "text/plain");
      }
    } else {
      res.status = 400;
      res.set_content("Missing parameters", "text/plain");
    }
  });

  // API: Send Message
  svr.Post("/send", [&](const Request &req, Response &res) {
    addCORS(res);
    if (req.has_param("sender") && req.has_param("receiver") &&
        req.has_param("content")) {
      string sender = req.get_param_value("sender");
      string receiver = req.get_param_value("receiver");
      string content = req.get_param_value("content");

      server.sendMessage(sender, receiver, content);
      res.set_content("Sent", "text/plain");
    } else {
      res.status = 400;
      res.set_content("Missing parameters", "text/plain");
    }
  });

  // API: Process Queue (and Mark Read)
  svr.Get("/process", [&](const Request &req, Response &res) {
    addCORS(res);
    string viewerId =
        req.has_param("viewer") ? req.get_param_value("viewer") : "";
    string currentChatId =
        req.has_param("chatWith") ? req.get_param_value("chatWith") : "";

    // If viewer is viewing a chat, mark messages from that chat as read
    if (!viewerId.empty() && !currentChatId.empty()) {
      server.markMessagesAsRead(currentChatId, viewerId);
      server.updateUserActivity(viewerId);
    }

    // Also update activity just for polling
    if (!viewerId.empty()) {
      server.updateUserActivity(viewerId);
    }

    auto messages = server.processMessagesAndReturnStructs();

    // Manual JSON serialization
    stringstream json;
    json << "[";
    for (size_t i = 0; i < messages.size(); ++i) {
      json << "{";
      json << "\"sender\": \"" << messages[i].senderId << "\",";
      json << "\"receiver\": \"" << messages[i].receiverId << "\",";
      json << "\"content\": \"" << messages[i].content << "\",";
      json << "\"timestamp\": \"" << messages[i].timestamp << "\",";
      json << "\"isRead\": " << (messages[i].isRead ? "true" : "false");
      json << "}";
      if (i < messages.size() - 1)
        json << ",";
    }
    json << "]";

    res.set_content(json.str(), "application/json");
  });

  // API: Get All Users (with Online Status)
  svr.Get("/users", [&](const Request &req, Response &res) {
    addCORS(res);
    string requesterId =
        req.has_param("requester") ? req.get_param_value("requester") : "";
    if (!requesterId.empty())
      server.updateUserActivity(requesterId);

    auto users = server.getAllUsers();
    long long now = time(nullptr);

    // Manual JSON serialization
    stringstream json;
    json << "[";
    for (size_t i = 0; i < users.size(); ++i) {
      bool isOnline =
          (now - users[i].lastActiveTime) < 15; // 15 seconds threshold

      json << "{";
      json << "\"id\": \"" << users[i].id << "\",";
      json << "\"name\": \"" << users[i].name << "\",";
      json << "\"online\": " << (isOnline ? "true" : "false");
      json << "}";
      if (i < users.size() - 1)
        json << ",";
    }
    json << "]";

    res.set_content(json.str(), "application/json");
  });

  cout << "Server started at http://localhost:8080" << endl;
  svr.listen("0.0.0.0", 8080);

  return 0;
}
