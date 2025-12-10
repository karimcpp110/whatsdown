#!/bin/bash

# WhatsDown Server Run Script (Linux/WSL)

echo "Starting WhatsDown Server..."

# Check if application is compiled
if [ ! -f "chatting_app" ]; then
    echo "Executable not found. Compiling..."
    # Try to clean up Windows newlines if any
    if [ -f "main.cpp" ]; then
        dos2unix *.cpp *.h > /dev/null 2>&1
    fi
    
    g++ main.cpp ChatController.cpp UserManager.cpp MessageManager.cpp -o chatting_app -pthread
    
    if [ $? -ne 0 ]; then
        echo "Compilation failed! Make sure you have g++ installed."
        read -p "Press Enter to exit..."
        exit 1
    fi
    echo "Compilation successful!"
fi

# Run the server
./chatting_app
