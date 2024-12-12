# Chat Application in C

A chat application implemented in C, enabling communication between a server and multiple clients. Users can set nicknames, view online users, send private messages, and participate in group channels.

---

## Features

- Set and manage user nicknames.
- View a list of online users.
- Send private messages to specific users.
- Communicate in group channels.
- Retrieve detailed user information.

---

## Requirements

- **CMake** (for building the project)

---

## How to Run

### Build the Project
1. Generate the build files:
   
   cmake CMakeLists.txt

## Start the Server
Run the server with a specified port:
./server <Port_Number>   

Run the client with the server's IP address and port:
./client <Server_IP_Address> <Port_Number>
