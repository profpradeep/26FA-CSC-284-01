// Assignment 06 - Multithreaded Chat Client
//
// Start with the supplied ChatClient.cpp. Complete the TODOs in order.
// Compile on macOS/Linux:
//   g++ -std=c++20 -Wall -Wextra -pthread ChatClient.cpp -o client
//
// Compile on Windows (MinGW):
//   g++ -std=c++20 -Wall -Wextra ChatClient.cpp -o client.exe -lws2_32

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    using Socket = SOCKET;
    constexpr Socket INVALID_SOCKET_VALUE = INVALID_SOCKET;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    using Socket = int;
    constexpr Socket INVALID_SOCKET_VALUE = -1;
    #define closesocket close
#endif

// TODO 1:
// Create shared connection state so the main thread can tell whether the
// receiving thread has detected a disconnection.
//
// Hint:
//   std::atomic<bool> connected{true};
//
// Also consider a mutex to keep output from the two threads from interleaving.

std::atomic<bool> connected{true};
std::mutex outputMutex;


// TODO 2:
// Move the original client's recv() code into this function.
// This function will run in the background receiving thread.
//
// Suggested function declaration:
//   void receiveMessages(Socket clientSocket)
//
// Steps:
//   1. Create a buffer.
//   2. While connected:
//        a. Call recv().
//        b. If bytesReceived > 0, display the incoming data.
//        c. If bytesReceived == 0, the server disconnected.
//        d. If bytesReceived < 0, handle the receive error.
//   3. Set connected to false when the receive loop ends.
//
// Important:
// Do not call getline() in this function. Its only job is receiving messages.

void receiveMessages(Socket clientSocket)
{
    // TODO 2: Implement the background receiving loop.
    
    // Set connected to false when the server disconnects or recv() fails.
}


// TODO 3:
// Parse and validate the optional port argument.
bool parsePort(const char* text, unsigned short& port)
{
    // TODO 3: Convert text to an integer and validate the range 1-65535.
    // Hint: std::stoi(text)
    
    return false;
}


int main(int argc, char* argv[])
{
    // TODO 4:
    // Support these command-line forms:
    //   ./client
    //   ./client <ip>
    //   ./client <ip> <port>
    //
    // Defaults:
    //   IP address: 127.0.0.1
    //   Port:       54000

    std::string ip = "127.0.0.1";
    unsigned short port = 54000;

    // TODO 4: Read argv values and validate the port.

#ifdef _WIN32
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    Socket clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == INVALID_SOCKET_VALUE)
    {
        std::cerr << "Failed to create socket\n";
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;

    // TODO 5:
    // Set the port and convert the IP address.
    //   serverAddr.sin_port = htons(port);
    //   inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(clientSocket,
                reinterpret_cast<sockaddr*>(&serverAddr),
                sizeof(serverAddr)) < 0)
    {
        std::cerr << "Connection failed\n";
        closesocket(clientSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    std::cout << "Connected to " << ip << ':' << port << '\n';
    std::cout << "Type messages. Enter 'quit' to exit.\n";

    // TODO 6:
    // Start a receiving thread AFTER the connection succeeds.
    //
    // Example:
    //   std::thread receiver(receiveMessages, clientSocket);

    // TODO 7:
    // Keep the main thread responsible for keyboard input and sending.
    //
    // The original starter client calls recv() after every send().
    // REMOVE that recv() from the main input loop.
    //
    // The main loop should:
    //   1. Prompt for input.
    //   2. Read a line using std::getline().
    //   3. Exit if the user enters "quit" or input ends.
    //   4. Send the line to the server.
    //
    // The receiver thread now handles all recv() calls.
    //
    // Hint: check connected in the loop condition.

    std::string input;

    while (connected)
    {
        std::cout << "> ";

        if (!std::getline(std::cin, input))
            break;

        if (input == "quit")
            break;

        // TODO 7: Send input to the server.
        // Check the return value of send().
    }

    // TODO 8:
    // Shut down the connection so a receiver blocked in recv() can wake up.
    //
    // Windows:
    //   shutdown(clientSocket, SD_BOTH);
    //
    // macOS/Linux:
    //   shutdown(clientSocket, SHUT_RDWR);
    //
    // Then join the receiving thread:
    //   receiver.join();
    //
    // Finally close the socket and call WSACleanup() on Windows.
    //
    // Do not call join() before shutdown(): recv() may otherwise wait forever.

    closesocket(clientSocket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
