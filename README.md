# WhatsDown - Premium Chat Platform

A real-time chatting application built with a robust **C++ Backend** and a stunning, glassmorphism **HTML/CSS/JS Frontend**.

## 🚀 Features

*   **💎 Glassmorphism UI**: A premium, modern interface with blur effects and smooth animations.
*   **🔒 Secure Accounts**: Password-protected login with automatic registration.
*   **🔔 Push Notifications**: Get system alerts for new messages, even when the app is minimized.
*   **🟢 Online Status**: See when your friends are online in real-time.
*   **👁️ Read Receipts**: Know exactly when your message has been seen (✔️ vs 👁️).
*   **🌗 Dark Mode**: Full dark theme support for late-night chatting.
*   **💾 Smart Persistence**: All chats and users are saved securely.

## 🛠️ Tech Stack

*   **Backend**: C++ (Custom Server Logic, File I/O) using `httplib.h`
*   **Frontend**: HTML5, Vanilla CSS3 (Glassmorphism), JavaScript (Fetch API)
*   **Networking**: SSH Tunneling (Pinggy.io) for public access

## 📦 How to Run

### 1. The Easy Way (WSL/Linux)
Open your terminal in the project folder and run:

```bash
# Compile and Run
wsl g++ main.cpp ChatController.cpp UserManager.cpp MessageManager.cpp -o chatting_app -lpthread
wsl ./chatting_app
```

The server will start at `http://localhost:8080`.

### 2. Connect via GitHub Pages (Public Access)
You can use our hosted frontend to connect to your local backend!

1.  **Start your Backend**: Run `./chatting_app` as above.
2.  **Expose to Internet**: Run the share script:
    ```bash
    bash share.sh
    ```
3.  **Copy the URL**: It will give you a link like `https://rand.pinggy.link`.
4.  **Open the App**: Go to [https://karimcpp110.github.io/whatsdown](https://karimcpp110.github.io/whatsdown).
5.  **Login**:
    *   Enter ID, Name, Password.
    *   **Paste the Server URL** from step 3.
    *   Click Start Chatting!

## 📱 Mobile Support
The app is fully responsive!
1.  Connect your phone to the same Wi-Fi.
2.  Open the browser on your phone.
3.  Go to `http://<YOUR_LAPTOP_IP>:8080`.
4.  Or use the **GitHub Pages** method above for access from anywhere (4G/5G).

## 📂 Project Structure

*   `main.cpp`: Backend Server Entry Point & API Routes.
*   `ChatController.*`: Logic for Auth, Messaging, and Activity.
*   `UserManager.*`: Database logic for User Accounts using `users.txt`.
*   `MessageManager.*`: Database logic for Messages using `messages.txt`.
*   `index.html`: The Application Shell.
*   `style.css`: The Premium Styling.
*   `script.js`: The Frontend Logic (API calls, Notifications).
