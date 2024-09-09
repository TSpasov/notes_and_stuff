#include "NetworkMgr.h"

#include <sys/socket.h> // For socket functions
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <poll.h> // For poll()
#include <vector> //
#include <algorithm> //

//https://notes.shichao.io/unp/ch1/#chapter-1-introduction

//I/O

namespace
{
  const size_t BUFFER_SIZE = 1024;
  const int PORT = 8080;
}

class NetworkError : public std::runtime_error {
public:
  explicit NetworkError(const std::string& message)
    : std::runtime_error(message) {}
};



NetworkMgr::NetworkMgr()
{

    // Create a socket (IPv4, TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
       throw NetworkError("Failed to create socket. errno: " );
    }

    // Listen to port 8080 on any address
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(PORT); // htons is necessary to convert a number to
        // network byte order
    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start listening. Hold at most 10 connections in the queue
    if (listen(sockfd, 10) < 0) {
        throw NetworkError("Failed to listen on socket. errno: " );
        exit(EXIT_FAILURE);
    }

    pollfd sock_poll_fd;
    sock_poll_fd.fd = sockfd;
    sock_poll_fd.events = POLLIN;
    fds.push_back(sock_poll_fd);
}

NetworkMgr::~NetworkMgr()
{
    close(sockfd);
}

void NetworkMgr::Process()
{

    int poll_count = poll(fds.data(), fds.size(), 100);
    if (poll_count < 0) {
        std::cout << "Failed to poll. errno: " << errno << std::endl;
    }

    // Check if there's a new connection
    if (fds[0].revents & POLLIN)
    {
        auto addrlen = sizeof(sockaddr);
        int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
        if (connection < 0)
        {
            std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        }

        // Add the new connection to the poll structure
        pollfd conn_poll_fd;
        conn_poll_fd.fd = connection;
        conn_poll_fd.events = POLLIN;
        fds.push_back(conn_poll_fd);

        m_on_accept(connection);
    }

    // Check all connections for data to read
    for (size_t i = 1; i < fds.size();)
    {
        if (fds[i].revents & POLLIN)
        {
            char buffer[BUFFER_SIZE] = {};

            auto bytesRead = read(fds[i].fd, buffer, BUFFER_SIZE);
            if (bytesRead <= 0)
            {

                // Remove the connection from the poll structure
                close(fds[i].fd);
                fds.erase(fds.begin() + i);
            }

            else
            {
                std::cout << " Client says  " << buffer << std::endl;
                // m_on_recieve(fds[i].fd, std::string(buffer));
                // Send a message to the connection
                 std::string response = "Good talking to you\n";
                 send(fds[i].fd, response.c_str(), response.size(), 0);

                ++i;
            }
        }
        else
        {
            ++i;
        }
    }
}

void NetworkMgr::Send(int clientID, const std::string& msg)
{
    send(clientID, msg.c_str(), msg.size(), 0);
}

void NetworkMgr::SendToAll(std::string msg)
{

    for (size_t i = 1; i < fds.size(); ++i)
    {
   //  send(fds[1].fd, msg.c_str(), msg.size(), 0);
    }
}

void NetworkMgr::RemoveConnection(int clientID)
{
    auto it = std::find_if(fds.begin(), fds.end(), [&](const pollfd& fd)
    {
       return fd.fd == clientID;
    });

    if(it != fds.end())
    {
        fds.erase(it);
        close(clientID);
    }
}

void NetworkMgr::OnAcceptConnection(std::function<void(int)> on_accept)
{
    m_on_accept = on_accept;
}

void NetworkMgr::OnRemoveConnection(std::function<void(int)> on_remove)
{
    m_on_remove = on_remove;
}

void NetworkMgr::OnRecieve(std::function<void(int, const std::string)> on_recieve)
{
    m_on_recieve = on_recieve;
}

bool NetworkMgr::HasConnections()
{
    return true;
}



// Listen for incoming connections
