# WhatsDown - Chatting Platform

A real-time chatting application built with a robust **C++ Backend** and a modern **HTML/CSS/JS Frontend**.

## 🚀 Features

*   **Secure Login**: University ID (`202X/XXXXX`) and Password authentication.
*   **Real-time Messaging**: Instant message delivery between users.
*   **Persistent Chat**: Messages are saved securely and retrieved upon login.
*   **Modern UI**: Sleek interface with dark mode support.
*   **Cross-Device**: Accessible from any device (Phone/Laptop) on the same network.

## 🛠️ Tech Stack

*   **Backend**: C++ (Custom Server Logic, File I/O for persistence)
*   **Server Library**: `httplib.h` (Single-header C++ HTTP server)
*   **Frontend**: HTML5, CSS3, Vanilla JavaScript

## 📦 How to Run

### Option 1: The Easy Way (Windows)
Simply double-click the **`run_server.bat`** file included in this folder.
*   It will launch the server automatically.
*   If the app isn't compiled yet, it will attempt to compile it for you (requires `g++`).

### Option 2: Manual Compilation
If you prefer the terminal, compile the code using `g++`:

```bash
g++ main.cpp ChatController.cpp UserManager.cpp MessageManager.cpp -o chatting_app -lws2_32 -static
```

Then run the application:
```bash
./chatting_app
```

## 🌐 How to Use

1.  **Start the Server** (using Option 1 or 2 above).
2.  Open your web browser (Chrome, Edge, Firefox).
3.  Go to: `http://localhost:8080`
4.  **Login/Register**:
    *   Enter your University ID (e.g., `2024/10001`).
    *   Enter a Name.
    *   Enter a Password (this will be set on first login).
5.  **Start Chatting**: Select a contact from the list and say hello!

### 📱 Connecting from Phone
To connect from your phone, ensure both devices are on the same Wi-Fi.
1.  Find your computer's IP address (Run `ipconfig` in cmd).
2.  On your phone, visit: `http://<YOUR_IP_ADDRESS>:8080`

## 📂 Project Structure

*   `main.cpp`: Entry point & Web Server setup.
*   `ChatController`: Manages user interactions and message flow.
*   `UserManager`: Handles user authentication and data.
*   `MessageManager`: Handles message storage and retrieval.
*   `index.html`: The main web interface.
*   `script.js`: Client-side logic.
*   `style.css`: Styling and layout.
