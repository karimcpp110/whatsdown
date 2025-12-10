#!/bin/bash
echo "========================================================"
echo "          WhatsDown Public Sharing Tool"
echo "========================================================"
echo "1. Ensure your server is running in another terminal (./chatting_app)"
echo "2. Keep this terminal OPEN to keep the link alive."
echo "========================================================"
echo "Generating Public Link..."
echo ""

# Use Pinggy.io to create a tunnel
# -p 443: Connect to port 443
# -R0:localhost:8080: Forward remote traffic to local port 8080
# qr@a.pinggy.io: The user/host (qr triggers QR code display)
ssh -p 443 -R0:localhost:8080 -o StrictHostKeyChecking=no qr@a.pinggy.io
