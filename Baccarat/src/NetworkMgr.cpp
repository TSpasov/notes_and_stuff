#include "NetworkMgr.h"

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

const char *SERVER_IP = "127.0.0.1";
const int PORT = 8080;
const int BUFFER_SIZE = 1024;


NetworkMgr::NetworkMgr()
{
    // Create socket
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_fd == -1)
    {
        // throw

    }
}

bool NetworkMgr::TryToConnect()
{


    struct sockaddr_in server_addr;
    // Initialize server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(m_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        return false;
    }

    std::cout << "Connected to server\n";


    // set nonblock I/O for m_fd
    int flags = fcntl(m_fd, F_GETFL, 0);
    fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
    m_Connected = true;
    return true;
}

void NetworkMgr::Process()
{
    if(!m_Connected)
    {
        return;
    }

    char buffer[BUFFER_SIZE] = {};

    const char *message = "Hello, server!";
    send(m_fd, message, strlen(message), 0);
    std::cout << "Message sent to server\n";

    // Receive response from server
    int bytes_received = recv(m_fd, buffer, BUFFER_SIZE, 0);

    // read errno  defined errno-base.h
    if (bytes_received == -1)
    {
        // if (errno == EAGAIN || errno == EWOULDBLOCK)
        // {
        //     // No data available right now, handle this appropriately
        // }
        // else
        // {
        //     // Some other error occurred, handle this appropriately
        // }
        m_Connected =false;
    }
    else
    {
        // Data received, process it
        buffer[bytes_received] = '\0';
        m_on_recieve(std::string(buffer));
    }
}


void NetworkMgr::OnAcceptConnection(std::function<void()> on_accept)
{
    m_on_accept = on_accept;
}

void NetworkMgr::OnRemoveConnection(std::function<void()> on_remove)
{
    m_on_remove = on_remove;
}

void NetworkMgr::OnRecieve(std::function<void(const std::string  msg)> on_recieve)
{
    m_on_recieve = on_recieve;
}

