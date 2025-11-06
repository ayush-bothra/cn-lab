#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    const char* SERVER_IP = "127.0.0.1";
    const int PORT = 54000;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    // htons is abbr for host to network short
    // here the PORT will get converted from host byte code
    // to a network byte code (big endian)
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }
    if (connect(
        sock,
        reinterpret_cast<sockaddr *>(&server_addr),
        sizeof(server_addr) 
    ) < 0) {
        perror("connect");
        return 1;
    }

    while (true) {
        std::string message;
        std::cout << "Type a message to send\n";
        /*std::getline take __CharT, __Traits, __Alloc
        (std allocator) of any defined type to take input
        below is the 2-arg version which calls the 3-arg version
        giving : __is, __str, __is.widen('\n')*/
        std::getline(std::cin, message);
        if (message == "quit") break;

        ssize_t sent = send(
            sock,
            message.c_str(), 
            message.size(),
            0
        );
        if (sent < 0) {
            perror("send");
            return 1;
        }

        const int BUF_SIZE = 1024;
        char buffer[BUF_SIZE];
        ssize_t n = recv(sock, buffer, BUF_SIZE - 1, 0);
        if (n < 0) {
            perror("recv");
            return 1;
        } else {
            buffer[n] = '\0';
            std:: cout << "Server reply: " << buffer << '\n';
        }
    }

    // close all files after use
    close(sock);
    return 0;
}