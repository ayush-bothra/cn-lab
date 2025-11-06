#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    const int PORT = 54001;
    // for datagrams the SOCK_type changes
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(
        sock,
        reinterpret_cast<sockaddr *>(&addr),
        sizeof(addr)
    ) < 0) {
        perror("bind");
        return 1;
    }

    std::cout << "UDP server listening on port: " << PORT << '\n';

    const int BUFF_SIZE = 1500;
    char buff[BUFF_SIZE];
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);

    while (true) {
        ssize_t n = recvfrom(sock, buff, BUFF_SIZE - 1, 0, 
            reinterpret_cast<sockaddr *>(&client_addr), &client_len);
        if (n < 0) {
            perror("recvfrom");
            break;
        } else {
            buff[n] = '\0';
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr,
            client_ip, sizeof(client_ip));
        std::cout   << "Recieved from client " << client_ip 
                    << " : " << ntohs(client_addr.sin_port)
                    << " : " << buff << '\n';

        ssize_t sent = sendto(sock, buff, BUFF_SIZE - 1, 0,
            reinterpret_cast<sockaddr *>(&client_addr),
            client_len
        );
        if (sent < 0) {
            perror("sendto");
            break;
        }
    }

    // close all files after done
    close(sock);
    return 0;
}