// #include <sys/socket.h> // For socket functions
// #include <netinet/in.h> // For sockaddr_in
// #include <cstdlib> // For exit() and EXIT_FAILURE
// #include <iostream> // For cout
// #include <unistd.h> // For read
// #include <poll.h> // For poll()
// #include <fcntl.h> // For fcntl()
// #include <arpa/inet.h> // For fcntl()

// int main() {
//     // Create a socket (IPv4, TCP)
//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd == -1) {
//         std::cout << "Failed to create socket. errno: " << errno << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     // Connect to the server at 127.0.0.1:9999
//     sockaddr_in sockaddr;
//     sockaddr.sin_family = AF_INET;
//     sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     sockaddr.sin_port = htons(9999);
//     if (connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
//         std::cout << "Failed to connect to server. errno: " << errno << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     // Set the socket to non-blocking mode
//     int flags = fcntl(sockfd, F_GETFL, 0);
//     fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

//     // Send a message to the server
//     std::string message = "Hello, server!\n";
//     if (write(sockfd, message.c_str(), message.size()) < 0) {
//         std::cout << "Failed to send message to server. errno: " << errno << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     // Set up poll structure
//     pollfd fds[2];
//     fds[0].fd = sockfd;
//     fds[0].events = POLLIN;
//     fds[1].fd = STDIN_FILENO;
//     fds[1].events = POLLIN;

//     while (true)
//     {
//         std::cout << " main loop " <<  std::endl;
//         // Poll for a response from the server (timeout: 5000 ms)
//         int poll_count = poll(fds, 2, 100);
//         if (poll_count < 0) {
//             std::cout << "Failed to poll. errno: " << errno << std::endl;
//             exit(EXIT_FAILURE);
//         }

//         if (fds[0].revents & POLLIN)
//         {
//             std::string message = "Hello, server!\n";
//             if (write(sockfd, message.c_str(), message.size()) < 0)
//             {
//                 std::cout << "Failed to send message to server. errno: " << errno << std::endl;
//                 exit(EXIT_FAILURE);
//             }
//             char buffer[100];
//             // Check if there's data to read
//             auto bytesRead = read(sockfd, buffer, 100);
//             if (bytesRead <= 0)
//             {
//                 // If read() fails with EWOULDBLOCK, it means that there is no data to
//                 // read and we can continue.
//                 if (errno != EWOULDBLOCK && bytesRead < 0)
//                 {
//                     std::cout << "Failed to read from server. errno: " << errno << std::endl;
//                     exit(EXIT_FAILURE);
//                 }
//                 if (bytesRead == 0)
//                 {
//                     // The server has closed the connection
//                     std::cout << "The server has closed the connection" << std::endl;
//                     break;
//                 }
//             } else
//             {
//                 std::cout << "The server said: " << buffer;
//             }
//         }


//         if (fds[1].revents & POLLIN)
//         {
//             char buffer[100];
//             auto bytesRead = read(STDIN_FILENO, buffer, 100);
//             if (bytesRead <= 0)
//             {
//                 // If read() fails with EWOULDBLOCK, it means that there is no data to
//                 // read and we can continue.
//                 if (errno != EWOULDBLOCK && bytesRead < 0)
//                 {
//                     std::cout << "Failed to read from standard input. errno: " << errno << std::endl;
//                     exit(EXIT_FAILURE);
//                 }
//                 // The user has closed the standard input (e.g. by pressing Ctrl+D)
//                 std::cout << "The user has closed the standard input" << std::endl;
//                 break;
//             }
//             else
//             {
//                 // Send the user's input to the server
//                 if (write(sockfd, buffer, bytesRead) < 0)
//                 {
//                     std::cout << "Failed to send message to server. errno: " << errno << std::endl;
//                     exit(EXIT_FAILURE);
//                 }
//             }
//         }
//         sleep(1);
//     }

//     // Close the connection
//     close(sockfd);
// }



#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h> // For inet_addr()
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <poll.h> // For poll()
#include <fcntl.h> // For fcntl()
#include <chrono> // For C++11's <chrono> library

int main() {
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Connect to the server at 127.0.0.1:9999
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  sockaddr.sin_port = htons(9999);
  if (connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
    std::cout << "Failed to connect to server. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Set the socket to non-blocking mode
  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

  // Send a message to the server
  std::string message = "Hello, server!\n";
  if (write(sockfd, message.c_str(), message.size()) < 0) {
    std::cout << "Failed to send message to server. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Set up poll structure
  pollfd fds;
  fds.fd = sockfd;
  fds.events = POLLIN;

  // Specify a timeout of 5000 milliseconds using <chrono>
  auto timeout = std::chrono::milliseconds(5000);

  while (true) {
    // Poll for a response from the server with the specified timeout
    int poll_count = poll(&fds, 1, timeout.count());
    if (poll_count < 0) {
      std::cout << "Failed to poll. errno: " << errno << std::endl;
      exit(EXIT_FAILURE);
    }

    // Check if there's data to read
    if (fds.revents & POLLIN) {
      char buffer[100];
      auto bytesRead = read(sockfd, buffer, 100);
      if (bytesRead <= 0) {
        // If read() fails with EWOULDBLOCK, it means that there is no data to
        // read and we can continue.
        if (errno != EWOULDBLOCK && bytesRead < 0) {
          std::cout << "Failed to read from server. errno: " << errno << std::endl;
          exit(EXIT_FAILURE);
        }
        if (bytesRead == 0) {
          // The server has closed the connection
          std::cout << "The server has closed the connection" << std::endl;
          break;
        }
      } else {
        std::cout << "The server said: " << buffer;
      }
    }
  }

  // Close the connection
  close(sockfd);
}
