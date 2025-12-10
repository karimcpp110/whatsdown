let myId = null;
let myName = null;
let currentChatId = null;
let currentChatName = null;
let displayedMessages = new Set();
let allUsers = [];
let apiBaseUrl = ""; // Default to local relative path

// Poll for messages and users
setInterval(() => fetchMessages(), 1000); // Poll faster for responsiveness
setInterval(() => fetchUsers(), 3000);

// Initialize features on load
document.addEventListener('DOMContentLoaded', () => {
    // Check Auto-Login
    const savedId = localStorage.getItem('whatsdown_id');
    const savedName = localStorage.getItem('whatsdown_name');
    const savedUrl = localStorage.getItem('whatsdown_url');

    if (savedId) {
        // Pre-fill Login Tab
        const loginIdField = document.getElementById('login-id');
        if (loginIdField) loginIdField.value = savedId;

        // Pre-fill Register Tab (optional)
        const regIdField = document.getElementById('reg-id');
        if (regIdField) regIdField.value = savedId;

        if (savedUrl) {
            const urlField = document.getElementById('server-url');
            if (urlField) urlField.value = savedUrl;
            apiBaseUrl = savedUrl;
        }
    }

    // Check Dark Mode
    const savedTheme = localStorage.getItem('whatsdown_theme');
    if (savedTheme === 'dark') {
        document.body.classList.add('dark-mode');
    }

    // Setup Search
    const searchInput = document.getElementById('search-input');
    if (searchInput) {
        searchInput.addEventListener('input', (e) => {
            filterContacts(e.target.value);
        });
    }

    // Setup Enter Key
    const input = document.getElementById('message-input');
    if (input) {
        input.addEventListener('keypress', function (e) {
            if (e.key === 'Enter') {
                sendMessage();
            }
        });
    }
});

// UI Toggles
function showLogin() {
    document.getElementById('form-login').classList.remove('hidden');
    document.getElementById('form-register').classList.add('hidden');
    document.getElementById('tab-login').classList.add('active');
    document.getElementById('tab-register').classList.remove('active');
}

function showRegister() {
    document.getElementById('form-login').classList.add('hidden');
    document.getElementById('form-register').classList.remove('hidden');
    document.getElementById('tab-login').classList.remove('active');
    document.getElementById('tab-register').classList.add('active');
}

// Common helper to save URL
function saveServerUrl() {
    const urlInput = document.getElementById('server-url').value.trim();
    apiBaseUrl = urlInput.replace(/\/$/, "");
    if (apiBaseUrl) {
        localStorage.setItem('whatsdown_url', apiBaseUrl);
    }
}

async function performLogin() {
    const id = document.getElementById('login-id').value;
    const password = document.getElementById('login-password').value;

    if (!id || !password) {
        alert("Please enter ID and Password");
        return;
    }

    saveServerUrl();

    // Enforce Server URL if using GitHub Pages (not localhost)
    if (!apiBaseUrl && window.location.hostname !== 'localhost' && window.location.hostname !== '127.0.0.1') {
        alert("⚠️ You are using the online version.\nPlease enter your Pinggy Server URL in the box below!");
        document.getElementById('server-url').focus();
        document.getElementById('server-url').style.border = "2px solid red";
        return;
    }

    const response = await fetch(`${apiBaseUrl}/login`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: `id=${id}&password=${encodeURIComponent(password)}`
    });

    if (!response.ok) {
        alert("Login Failed: " + (await response.text()));
        return;
    }

    // Success
    const name = await response.text();
    completeLogin(id, name);
}

async function performRegister() {
    const id = document.getElementById('reg-id').value;
    const name = document.getElementById('reg-name').value;
    const password = document.getElementById('reg-password').value;

    if (!id || !name || !password) {
        alert("Please enter all details");
        return;
    }

    saveServerUrl();

    // Enforce Server URL if using GitHub Pages (not localhost)
    if (!apiBaseUrl && window.location.hostname !== 'localhost' && window.location.hostname !== '127.0.0.1') {
        alert("⚠️ You are using the online version.\nPlease enter your Pinggy Server URL in the box below!");
        document.getElementById('server-url').focus();
        document.getElementById('server-url').style.border = "2px solid red";
        return;
    }

    const response = await fetch(`${apiBaseUrl}/register`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: `id=${id}&name=${encodeURIComponent(name)}&password=${encodeURIComponent(password)}`
    });

    if (!response.ok) {
        alert("Registration Failed: " + (await response.text()));
        return;
    }

    // Success - Auto Login
    alert("Account Created! Logging in...");
    completeLogin(id, name);
}

function completeLogin(id, name) {
    myId = id;
    myName = name;

    localStorage.setItem('whatsdown_id', myId);
    localStorage.setItem('whatsdown_name', myName);

    // Switch UI
    document.getElementById('login-screen').classList.add('hidden');
    document.getElementById('app-screen').classList.remove('hidden');
    document.getElementById('display-name').textContent = myName;
    document.getElementById('my-avatar').textContent = myName.charAt(0).toUpperCase();
    document.getElementById('my-avatar').style.background = getUserColor(myId);

    // Request Notification Permission
    if ("Notification" in window) {
        Notification.requestPermission();
    }

    // Initial fetch
    fetchUsers();
}

function logout() {
    localStorage.removeItem('whatsdown_id');
    localStorage.removeItem('whatsdown_name');
    location.reload();
}

function toggleTheme() {
    document.body.classList.toggle('dark-mode');
    const isDark = document.body.classList.contains('dark-mode');
    localStorage.setItem('whatsdown_theme', isDark ? 'dark' : 'light');
}

function insertEmoji(emoji) {
    const input = document.getElementById('message-input');
    input.value += emoji;
    input.focus();
}

function clearChat() {
    if (confirm("Clear this chat view? (Messages are still saved on server)")) {
        document.getElementById('messages-list').innerHTML = '';
        displayedMessages.clear();
    }
}

function filterContacts(query) {
    const list = document.getElementById('contact-list');
    list.innerHTML = '';

    const lowerQuery = query.toLowerCase();
    const filtered = allUsers.filter(u =>
        u.id !== myId &&
        (u.name.toLowerCase().includes(lowerQuery) || u.id.toString().includes(lowerQuery))
    );

    if (filtered.length === 0) {
        list.innerHTML = '<div style="padding: 20px; text-align: center; color: var(--text-secondary);">No contacts found</div>';
        return;
    }

    filtered.forEach(user => {
        renderContact(user, list);
    });
}

function renderContact(user, container) {
    const div = document.createElement('div');
    div.className = `contact ${user.id === currentChatId ? 'active' : ''}`;
    div.dataset.id = user.id;
    div.onclick = () => selectContact(user.id, user.name);

    const statusColor = user.online ? '#25d366' : '#ccc'; // Green for Online, Grey for Offline

    div.innerHTML = `
        <div class="avatar" style="background: ${getUserColor(user.id)}">${user.name.charAt(0).toUpperCase()}</div>
        <div class="contact-info">
            <div class="name">${user.name}</div>
            <div class="last-msg" style="display:flex; align-items:center;">
                <span class="status-dot" style="background:${statusColor}; width:10px; height:10px; border-radius:50%; display:inline-block; margin-right:5px;"></span>
                <span>${user.online ? 'Online' : 'Offline'}</span>
            </div>
        </div>
    `;
    container.appendChild(div);
}

function selectContact(id, name) {
    currentChatId = id;
    currentChatName = name;

    document.getElementById('welcome-screen').classList.add('hidden');
    document.getElementById('chat-interface').classList.remove('hidden');

    document.getElementById('current-contact-name').textContent = name;
    document.getElementById('current-contact-avatar').textContent = name.charAt(0).toUpperCase();
    document.getElementById('current-contact-avatar').style.background = getUserColor(id);

    document.getElementById('messages-list').innerHTML = '';
    displayedMessages.clear();

    document.querySelectorAll('.contact').forEach(el => el.classList.remove('active'));

    const contactEl = document.querySelector(`.contact[data-id="${id}"]`);
    if (contactEl) contactEl.classList.add('active');

    // Mobile: Show chat area
    document.getElementById('app-screen').classList.add('chat-active');

    fetchMessages();
}

function showContactList() {
    currentChatId = null;
    document.getElementById('app-screen').classList.remove('chat-active');
}

async function sendMessage() {
    if (!currentChatId) {
        alert("Select a contact first!");
        return;
    }

    const input = document.getElementById('message-input');
    const content = input.value.trim();

    if (!content) return;

    input.value = '';

    await fetch(`${apiBaseUrl}/send`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: `sender=${myId}&receiver=${currentChatId}&content=${encodeURIComponent(content)}`
    });

    fetchMessages();
}

async function fetchMessages() {
    if (!myId) return;

    try {
        // Send 'chatWith' param to mark messages as read
        const url = `${apiBaseUrl}/process?viewer=${myId}&chatWith=${currentChatId || ''}`;
        const response = await fetch(url);
        const messages = await response.json();
        let newMessages = false;

        if (!currentChatId) return; // Don't render chat if no chat selected, but loop runs to keep session alive

        messages.forEach(msg => {
            // Unique ID includes read status so we update ticks if they change
            const msgId = `${msg.sender}-${msg.receiver}-${msg.content}-${msg.timestamp}`;

            const existingBubble = document.getElementById(msgId);

            if (msg.receiver === myId && msg.sender === currentChatId) {
                if (!existingBubble) {
                    addMessageBubble(msg.content, 'received', msg.timestamp, msg.isRead, msgId);
                    displayedMessages.add(msgId);
                    newMessages = true;
                }
            } else if (msg.sender === myId && msg.receiver === currentChatId) {
                if (!existingBubble) {
                    addMessageBubble(msg.content, 'sent', msg.timestamp, msg.isRead, msgId);
                    displayedMessages.add(msgId);
                } else {
                    // Check if we need to update read receipt
                    const tickSpan = existingBubble.querySelector('.ticks');
                    if (tickSpan && msg.isRead && tickSpan.textContent !== '👁️') {
                        tickSpan.textContent = '👁️';
                        tickSpan.style.color = '#34b7f1';
                    }
                }
            }
        });

        if (newMessages) {
            const audio = document.getElementById('notification-sound');
            if (audio) {
                audio.play().catch(e => { });
            }

            // Show System Notification if backgrounded
            if (document.hidden && "Notification" in window && Notification.permission === "granted") {
                new Notification("WhatsDown", { body: "New message received!" });
            }
        }

    } catch (e) {
        console.error("Error fetching messages:", e);
    }
}

async function fetchUsers() {
    if (!myId) return;

    try {
        // Pass requester ID to update activity
        const response = await fetch(`${apiBaseUrl}/users?requester=${myId}`);
        const users = await response.json();

        allUsers = users;

        // Update contact list (re-render to update online status)
        const searchInput = document.getElementById('search-input');
        if (!searchInput || !searchInput.value) {
            filterContacts('');
        }

        // Also update header status if current chat is the one
        if (currentChatId) {
            const user = allUsers.find(u => u.id == currentChatId);
            if (user) {
                const statusDiv = document.querySelector('.chat-info .status');
                if (statusDiv) statusDiv.textContent = user.online ? 'Online' : 'Offline';
            }
        }

    } catch (e) {
        console.error("Error fetching users:", e);
    }
}

function addMessageBubble(text, type, timestamp, isRead, id) {
    const container = document.getElementById('messages-list');

    const bubble = document.createElement('div');
    bubble.className = `message-bubble ${type}`;
    bubble.id = id;

    const time = timestamp || new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    const ticks = type === 'sent' ? (isRead ? '👁️' : '✔️') : '';
    const tickColor = isRead ? '#34b7f1' : '#999';

    bubble.innerHTML = `
        ${text}
        <div style="display:flex; justify-content:flex-end; align-items:center; margin-top:5px;">
            <span class="time" style="margin:0; margin-right:5px;">${time}</span>
            <span class="ticks" style="font-size:0.8rem; color:${tickColor}">${ticks}</span>
        </div>
    `;

    container.appendChild(bubble);
    container.scrollTop = container.scrollHeight;
}

// Generate a consistent color from string
function getUserColor(str) {
    let hash = 0;
    for (let i = 0; i < str.length; i++) {
        hash = str.charCodeAt(i) + ((hash << 5) - hash);
    }
    const c = (hash & 0x00FFFFFF).toString(16).toUpperCase();
    return '#' + "00000".substring(0, 6 - c.length) + c;
}
