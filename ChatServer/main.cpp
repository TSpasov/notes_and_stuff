#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <poll.h> // For poll()
#include <vector> // For vector

int main() {
    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen to port 9999 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(9999); // htons is necessary to convert a number to
    // network byte order
    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start listening. Hold at most 10 connections in the queue
    if (listen(sockfd, 10) < 0) {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set up poll structure
    std::vector<pollfd> fds;
    pollfd sock_poll_fd;
    sock_poll_fd.fd = sockfd;
    sock_poll_fd.events = POLLIN;
    fds.push_back(sock_poll_fd);

    while (true)
    {
        std::cout << " Main loop " << std::endl;
        // Wait for an event (timeout: -1 means wait indefinitely)
        int poll_count = poll(fds.data(), fds.size(), 100);
        if (poll_count < 0)
        {
            std::cout << "Failed to poll. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }

        // Check if there's a new connection
        if (fds[0].revents & POLLIN)
        {
            sockaddr_in client_sockaddr;
            socklen_t client_sockaddr_len = sizeof(client_sockaddr);
            int connection = accept(sockfd, (struct sockaddr*)&client_sockaddr, &client_sockaddr_len);
            if (connection < 0)
            {
                std::cout << "Failed to grab connection. errno: " << errno << std::endl;
                exit(EXIT_FAILURE);
            }

            // Add the new connection to the poll structure
            pollfd conn_poll_fd;
            conn_poll_fd.fd = connection;
            conn_poll_fd.events = POLLIN;
            fds.push_back(conn_poll_fd);
        }

        // Check all connections for data to read
        for (size_t i = 1; i < fds.size();)
        {
            if (fds[i].revents & POLLIN) {
                char buffer[100];
                auto bytesRead = read(fds[i].fd, buffer, 100);
                if (bytesRead <= 0)
                {
                    // Remove the connection from the poll structure
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                }
                else
                {
                    std::cout << "The message was: " << buffer << "from client " << fds[i].fd;



                    ++i;
                }
            }
            else
            {
                // // Send a message to the connection
                // std::string response = "Good talking to you ";

                // response += std::to_string(fds[i].fd);
                // send(fds[i].fd, response.c_str(), response.size(), 0);
                ++i;
            }
        }

        sleep(1);
    }

    close(sockfd);
}
