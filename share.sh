#!/bin/bash
echo "========================================================"
echo "          WhatsDown Public Sharing Tool"
echo "========================================================"
echo "1. Ensure your server is running in another terminal (./chatting_app)"
echo "2. Keep this terminal OPEN to keep the link alive."
echo "========================================================"
echo "Generating Public Link..."
echo ""

# Switch to localhost.run as it doesn't require a password
echo "Attempting to create tunnel using localhost.run..."
ssh -R 80:localhost:8080 nokey@localhost.run
