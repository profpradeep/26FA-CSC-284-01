// Assignment 06 - Multi-Client Chat Server
//
// Start with the supplied ChatServer.cpp. Complete the TODOs in order.
// Compile on macOS/Linux:
//   g++ -std=c++20 -Wall -Wextra -pthread ChatServer.cpp -o server
//
// Compile on Windows (MinGW):
//   g++ -std=c++20 -Wall -Wextra ChatServer.cpp -o server.exe -lws2_32

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <algorithm>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    using Socket = SOCKET;
    using SocketLength = int;
    constexpr Socket INVALID_SOCKET_VALUE = INVALID_SOCKET;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <csignal>
    using Socket = int;
    using SocketLength = socklen_t;
    constexpr Socket INVALID_SOCKET_VALUE = -1;
    #define closesocket close
#endif

// TODO 1:
// Create a shared collection that stores the socket for every connected client.
// Create a mutex to protect that collection.
//
// Hint:
//   std::vector<Socket> clients;
//   std::mutex clientsMutex;


// TODO 2:
// Write a function that sends a message to every connected client EXCEPT
// the client that originally sent the message.
//
// Suggested function declaration:
//   void broadcastMessage(Socket senderSocket, const std::string& message)
//
// Steps:
//   1. Lock the shared client collection.
//   2. Loop through the connected client sockets.
//   3. Skip senderSocket.
//   4. Send the message to every other client.
//   5. Check whether send() failed.
//
// Important:
// For this starter version, keep the mutex locked while iterating and sending
// so a socket is not removed/closed while it is being used. This is simple
// for the assignment, but a slow client can temporarily delay other clients.
// A more advanced version would use a safer per-client synchronization design.

void broadcastMessage(Socket senderSocket, const std::string& message)
{
    // TODO 2: Implement broadcasting.
}


// TODO 3:
// Write a function that removes a disconnected client from the shared vector.
//
// Suggested function declaration:
//   void removeClient(Socket clientSocket)
//
// Steps:
//   1. Lock clientsMutex.
//   2. Find clientSocket in clients.
//   3. Remove it from the vector.
//   4. Release the lock.
//   5. Close the socket.
//
// Be careful not to close the same socket more than once.

void removeClient(Socket clientSocket)
{
    // TODO 3: Implement client removal and socket cleanup.
}


// TODO 4:
// Move the original server's recv() loop into this function.
// This function will run in a separate thread for ONE connected client.
//
// Suggested function declaration:
//   void handleClient(Socket clientSocket)
//
// Steps:
//   1. Create a buffer.
//   2. Repeatedly call recv() on clientSocket.
//   3. If recv() returns a positive number:
//        a. Build a std::string using exactly bytesReceived bytes.
//        b. Display the message on the server.
//        c. Call broadcastMessage(clientSocket, message).
//   4. If recv() returns 0, the client disconnected.
//   5. If recv() returns a negative number, handle the error.
//   6. After the loop, call removeClient(clientSocket).
//
// Note: recv() returns bytes, not necessarily one complete application-level
// message. For this assignment's basic starter protocol, treat each received
// chunk as the data to broadcast. TCP message framing can be improved later.

void handleClient(Socket clientSocket)
{
    // TODO 4: Implement the receive loop for this client.
}


// TODO 5:
// Parse the optional port argument.
// Return true if it is a valid port (1-65535); otherwise return false.
bool parsePort(const char* text, unsigned short& port)
{
    // TODO 5: Convert text to an integer and validate the range.
    // Hint: std::stoi(text)
    return false;
}


int main(int argc, char* argv[])
{
#ifndef _WIN32
    // Prevent the program from terminating if send() targets a disconnected
    // client. Windows Winsock does not use SIGPIPE.
    std::signal(SIGPIPE, SIG_IGN);
#endif

    // TODO 6:
    // Support these command-line forms:
    //   ./server
    //   ./server <ip>
    //   ./server <ip> <port>
    //
    // Defaults:
    //   IP address: 127.0.0.1
    //   Port:       54000
    //
    // Hint:
    //   std::string ip = "127.0.0.1";
    //   unsigned short port = 54000;
    //   argc tells you how many arguments were supplied.

    std::string ip = "127.0.0.1";
    unsigned short port = 54000;

    // TODO 6: Read argv values and validate the port.

#ifdef _WIN32
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    // Create the listening socket.
    Socket listenSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (listenSocket == INVALID_SOCKET_VALUE)
    {
        std::cerr << "Cannot create socket\n";
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;

    // TODO 7:
    // Set the port using htons(port).
    // Convert the IP string into serverAddr.sin_addr using inet_pton().
    //
    // Example:
    //   serverAddr.sin_port = htons(port);
    //   inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY; // TODO 7: Replace with supplied/default IP.

    if (bind(listenSocket,
             reinterpret_cast<sockaddr*>(&serverAddr),
             sizeof(serverAddr)) < 0)
    {
        std::cerr << "Bind failed\n";
        closesocket(listenSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) < 0)
    {
        std::cerr << "Listen failed\n";
        closesocket(listenSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    std::cout << "Server listening on " << ip << ':' << port << '\n';

    // TODO 8:
    // The original starter server calls accept() only once and then closes
    // the listening socket. Change this to an infinite loop.
    //
    // For every successful accept():
    //   1. Display the client's IP address and port.
    //   2. Add clientSocket to the shared clients vector (under the mutex).
    //   3. Create a new thread:
    //        std::thread worker(handleClient, clientSocket);
    //   4. Decide how the server will manage that thread.
    //
    // For this assignment, detach() is one possible approach:
    //        worker.detach();
    //
    // The main server thread must return to accept() so it can accept
    // another client while existing client threads continue running.

    while (true)
    {
        sockaddr_in clientAddr{};
        SocketLength clientSize = sizeof(clientAddr);

        Socket clientSocket = accept(
            listenSocket,
            reinterpret_cast<sockaddr*>(&clientAddr),
            &clientSize
        );

        if (clientSocket == INVALID_SOCKET_VALUE)
        {
            std::cerr << "Accept failed\n";
            continue;
        }

        char clientIp[INET_ADDRSTRLEN]{};
        inet_ntop(AF_INET, &clientAddr.sin_addr,
                  clientIp, sizeof(clientIp));

        std::cout << "Client connected from "
                  << clientIp << ':' << ntohs(clientAddr.sin_port) << '\n';

        // TODO 8a: Add clientSocket to the shared clients vector.
        // Remember to lock clientsMutex.

        // TODO 8b: Start a new thread to handle this client.
        // Pass clientSocket to handleClient().
        // Detach or otherwise manage the thread.
    }

    // The loop above runs until the server is stopped.
    // A production server would also need an orderly server-shutdown strategy.
    closesocket(listenSocket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
