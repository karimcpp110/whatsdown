# How to Access ChatApp on Your Phone

Because the app is running inside **WSL (Linux)**, it is hidden from your local network by default. To access it from your phone, you need to "bridge" the connection to Windows.

## Step 1: Run PowerShell as Administrator
1. Press `Win` key.
2. Type `PowerShell`.
3. Right-click and select **Run as Administrator**.

## Step 2: Run the Bridge Command
Copy and paste this command into the Administrator PowerShell window:

```powershell
netsh interface portproxy add v4tov4 listenport=8080 listenaddress=0.0.0.0 connectport=8080 connectaddress=172.30.189.94
```

> **Note:** `172.30.189.94` is the IP address of your WSL instance I found earlier. If you restart WSL, this might change.

## Step 3: Allow through Firewall
Run this command to allow the port through Windows Firewall:

```powershell
New-NetFirewallRule -DisplayName "ChatApp Port 8080" -Direction Inbound -LocalPort 8080 -Protocol TCP -Action Allow
```

## Step 4: Find Your Computer's IP
Run this command to see your Wi-Fi IP address:

```powershell
ipconfig
```

Look for **IPv4 Address** under "Wireless LAN adapter Wi-Fi". It will look like `192.168.1.5` (or similar).

## Step 5: Connect on Phone
1. Ensure your phone is on the **same Wi-Fi** as your computer.
2. Open your phone's browser.
3. Go to: `http://<YOUR_COMPUTER_IP>:8080`
   (Example: `http://192.168.1.5:8080`)
