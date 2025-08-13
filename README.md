# FT_IRC

![C++98](https://img.shields.io/badge/C%2B%2B-98-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![License](https://img.shields.io/badge/license-Educational-lightgrey.svg)

## 📌 Overview
`ft_irc` is an IRC (Internet Relay Chat) server written in **C++98**, following the [RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459) specification.  
It allows multiple clients to connect using standard IRC clients (e.g., HexChat, irssi, weechat) to chat in real-time through channels or direct messages.

This project was developed as part of the 42 school curriculum to practice:
- Socket programming
- TCP/IP networking
- Event-driven architecture using `poll()`
- Parsing text-based protocols
- Implementing a real-world standard (IRC protocol)

---

## ⚙️ Features
- **Multi-client support** — Handle multiple users simultaneously.
- **Channel creation & management** — Join, leave, topic, modes, kicks, invites.
- **Authentication** — Clients must use `PASS`, `NICK`, and `USER` before chatting.
- **Private messaging** — Direct user-to-user messages.
- **Operator commands** — Admin-level privileges for managing channels.
- **RFC-compliant message parsing** — Handles prefix, parameters, and trailing data.

---

## 🛠️ Installation

### 1️⃣ Clone the repository
```bash
git clone https://github.com/banditklm/Ft_IRC/.git
cd ft_irc
```

### 2️⃣ Build the server
```bash
make
```

## 🚀 Usage

### Start the server
```bash
./ircserv <port> <password>
```
<port> — Port number to listen on (e.g., 6667).

<password> — Server password required for PASS command.

### Connect with an IRC client
```bash
 nc 127.0.0.1 6667
 PASS mypassword
 NICK kaltoum
 USER kaltoum 0 * :Kaltoum Elmounjid
 JOIN #42network
 PRIVMSG #42network :Hello everyone!
```
### Example server response:
```bash
:irc.com 001 kaltoum :Welcome to the Internet Relay Network kaltoum!kaltoum@irc.com
:irc.com 002 kaltoum :Your host is irc.com, running version 1.0
:irc.com 003 kaltoum :This server was created Wed Aug 13 2025 18:44:56
:irc.com 004 kaltoum irc.com 1.0 i it
:kaltoum!kaltoum@host JOIN :#42network
:irc.com 353 kaltoum = #42network :@kaltoum
:irc.com 366 kaltoum #42network :End of /NAMES list.

```

## 📜 Supported Commands

| Command   | Description                                |
| --------- | ------------------------------------------ |
| `PASS`    | Set connection password (must be first).   |
| `NICK`    | Set nickname.                              |
| `USER`    | Set username and real name.                |
| `JOIN`    | Join a channel (`JOIN #channel`).          |
| `PRIVMSG` | Send private message to a user or channel. |
| `KICK`    | Kick a user from a channel.                |
| `INVITE`  | Invite a user to a channel.                |
| `TOPIC`   | Set or view channel topic.                 |
| `MODE`    | Set channel/user modes.                    |

## 🔍 How It Works
1. Startup — The server creates a socket, binds it to the given port, and starts listening for incoming connections.
2. Event loop with `poll()` — The server continuously monitors multiple file descriptors for:
   - New client connections
   - Incoming messages from existing clients
3. Command parsing — Each IRC message is split into:
   - Prefix (optional)
   - Command (e.g., NICK, JOIN, PRIVMSG)
   - Parameters (space-separated arguments, trailing ":message")
4. Command execution — The server validates the command and updates internal structures (clients, channels, modes) accordingly.
5. Response — The server sends back numeric replies or broadcasts messages to appropriate recipients.

## 📂 Project Structure
```bash
ft_irc/
│── src/
│── client/
│── server/
│── headers/
│── Makefile
│── README.md
```
## 📚 References
- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [Guide to Network Programming]((https://www.tutorialspoint.com/unix_sockets/socket_core_functions.htm))
- [IRC Command Reference](https://modern.ircdocs.horse/)

## 👨‍💻 Authors
- **Kaltoum Elmounjid** — [GitHub](https://github.com/banditklm)














