
# Assignment: Multi-Client Chat Server and Client in C++

## Objective

In this assignment, you will extend the provided starter code for the chat **server** and **client** (located [here](https://github.com/profpradeep/26FA-CSC-284-01/blob/main/content/chatroom)) to support multiple connected clients communicating simultaneously.

The server must be capable of handling multiple clients concurrently and broadcasting messages between them.

The client must be converted into a multithreaded program so that it can receive and display incoming messages while the user continues typing and sending messages.

---

## Part 1: Multi-Client Chat Server

You will modify the *starter server code* so that it becomes a functioning multi-client chat server.

### Server Requirements

#### 1. Support Multiple Client Connections

- The server must accept simultaneous connections from multiple clients.
- Each client should be handled in its own thread (or equivalent asynchronous structure).
- The server must not block other clients from connecting while it handles one client.

#### 2. Broadcast Incoming Messages

- When **any** connected client sends a text message, the server must broadcast that message to **all other connected clients**.
- The sender should *not* need to be identified in the message (e.g., no usernames required yet).
- If a client disconnects, the server should cleanly remove it from the list of active clients.

#### 3. Allow IP Address and Port to Be Specified as Command-Line Arguments

The server must support the following command-line formats:

```bash
./server
./server <ip>
./server <ip> <port>
```

If arguments are omitted, use the following defaults:

- **Default IP:** `127.0.0.1`
- **Default port:** `54000`

#### 4. Maintain a List of Active Client Sockets

- Store client sockets in a shared container.
- Use a mutex (or platform-appropriate synchronization) to protect the list during:
  - Additions
  - Removals
  - Broadcasting

#### 5. Clean Shutdown for Client Threads

When a client disconnects (`recv()` returns `0` or a value less than `0`):

- Close the socket.
- Remove it from the active client list.
- End its thread cleanly.

---

## Part 2: Updated Multithreaded Client

You will extend the provided *starter client code* to support sending and receiving messages simultaneously.

The client must use multiple threads so that receiving messages from the server does not prevent the user from typing and sending messages.

### Client Requirements

#### 1. Connect to the Server

The client must connect to the server using the following command-line formats:

```bash
./client
./client <ip>
./client <ip> <port>
```

Use the same default IP and port values as the server:

- **Default IP:** `127.0.0.1`
- **Default port:** `54000`

#### 2. Send Messages Typed by the User

- The main thread should handle user input.
- Each line entered by the user should be sent to the server exactly as typed.
- The user must be able to continue typing and sending messages while incoming messages are being received.

#### 3. Receive and Display Messages from the Server

- A separate background thread must continuously call `recv()` to receive messages from the server.
- Incoming messages must be displayed immediately.
- Receiving a message must not block or interfere with user input.

#### 4. Handle Disconnections

- If the server disconnects or the connection is lost, the client should detect the disconnection and terminate cleanly.
- The receiving thread must not continue running after the connection is closed.
- The client should close the socket and clean up its threads before exiting.

---

## Submission Notes

### 1. IP Address Testing

Do not worry about testing the IP address value passed into either the client or the server.

It is important that the IP address command-line argument, if specified, is used by the programs appropriately. However, you do not have to actually test that it works correctly.

**Only the port numbers need to work as expected.**

### 2. Submission Folder Structure

Your folder structure should look like this:

```text
Assignment_06/
    ├── Server/
    │   └── (server source files)
    └── Client/
        └── (client source files)
```

**Note:** The code in this folder will be evaluated for this assignment only.

As the capstone project evolves, you may copy this code for future assignments and refine it further.