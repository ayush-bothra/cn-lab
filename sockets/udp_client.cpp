#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    const char *SERVER_IP = "127.0.0.1";
    const int PORT = 54001;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_port = htons(PORT);
    server_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) < 0) {
        perror("inet_ptons");
        return 1;
    }

    while (true) {
        std::string message;
        std::cout << "Write a message to be sent\n";
        std::getline(std::cin, message);
        if (message == "quit") break;
        /*sendto is a C func, it requires raw pointers
        ie void pointers and the likes, since string is a obj
        it wont degrade to void *buf but degrade to char* buf
        hence we use either message.c_str() (dangerous)
        or use msg.data() safer.*/
        ssize_t sent = sendto(sock, message.data(), message.size(), 0,
            reinterpret_cast<sockaddr *>(&server_addr),
            sizeof(server_addr)
        );
        if (sent < 0) {
            perror("sendto");
            return 1;
        }

        const int BUF_SIZE = 1500;
        char buf[BUF_SIZE];
        sockaddr_in from_addr{};
        socklen_t from_len = sizeof(from_addr);

        ssize_t n = recvfrom(sock, buf, BUF_SIZE - 1, 0,
            reinterpret_cast<sockaddr *>(&from_addr),
            &from_len
        );
        if (n < 0) {
            perror("recvfrom");
            return 1;
        } else {
            buf[n] = '\0';
            std::cout << "Reply: " << buf << '\n';
        }
    }
    

    // close files after work is done
    close(sock);
    return 0;
}