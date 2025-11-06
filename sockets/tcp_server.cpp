// net access utils and protocols
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
// defines IP and structure and socket options
#include <netinet/in.h>
// defines the core socket APIs
#include <sys/socket.h>
// POSIX OS interface header for syscalls
#include <unistd.h>


int main() {
    const int PORT = 54000;
    /* UNIX treats sockets like files, so when we 
     create a socket, it will return int handle for it
     the fd stands for file descriptor, 
     returns -1 for errors, if protocol is 0, auto choose */
    // listen_fd is only there for incoming connections.
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    /* Print a message describing the meaning of the value of errno.

    This function is a possible cancellation point and therefore not
    marked with __THROW.  */
    if (listen_fd < 0) {
        perror("socket");
        return 1;
    }
    /* Set socket FD's option OPTNAME at protocol level LEVEL
    to *OPTVAL (which is OPTLEN bytes long).
    Returns 0 on success, -1 for errors.  */
    int opt = 1;
    // void ptrs used for flexible casting to reqd ptrs.
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    /*#define	__SOCKADDR_COMMON(sa_prefix) \
    sa_family_t sa_prefix##family
    this code adds the prefix to the macro, acc to usage
    above struct particularly for sin_ prefix*/
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    // converts to big endian (network protocol) and send to PORT
    // abbr of host-to-network short
    addr.sin_port = htons(PORT);
    // socket APIs work using pointers, addr is a struct on stack
    // so we pass a pointer of its to the API (the & references the adr.)
    // the reinterpret_cast casts between unrelated structs safely
    if (bind(
        listen_fd, 
        reinterpret_cast<sockaddr*>(&addr), 
        sizeof(addr)) < 0) {
            perror("bind");
            return 1;
    }
    // here, 5 connection reqs will be sent before timeout
    if (listen(listen_fd, 5) < 0) {
        perror("listen");
        return 1;
    }

    std::cout << "TCP server listening on port..." << PORT << '\n';

    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    /* Await a connection on socket FD.
    When a connection arrives, open a new socket to communicate with it,
    set *client_addr (which is *client_len bytes long) to the address 
    of the connecting peer and *client_len to the address's actual length, 
    and return the new socket's descriptor, or -1 for errors.

    This function is a cancellation point and therefore not marked with
    __THROW.  */
    int client_fd = accept(
                        listen_fd, 
                        reinterpret_cast<sockaddr *>(&client_addr),
                        &client_len
                    );
    if (client_fd < 0) {
        perror("accept");
        return 1;
    }
    // the __cp (second arg) is pointer to the binary net addr
    // in case of AF_INET, it shld be in_addr, of client
    // again void pointers used for flexibility
    // client_ip is the __buf (buffer) with len -> __LEN
    char client_ip[INET_ADDRSTRLEN];
    // above char array is buf for single IPv4 string
    inet_ntop(AF_INET, &client_addr.sin_addr, 
                client_ip, sizeof(client_ip));
    // ntohs takes a netshort -> net PORT, uint short size.
    // abbrv for network to host short (uint16_t);
    // conversion from net byte order to host byte order
    std::cout   << "Client connected: " << client_ip << ":" 
                << ntohs(client_addr.sin_port) << "\n";

    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];
    while (true) {
        // recieves valid addr length or -1 for errors,
        // hence the ssize_t
        /*client_fd is the connection to recieve from
        the buf stores this recv value, BUF_SIZE is reduced
        by 1 to avoid null termination overflows, flag = 0
        means normal blocking recieve*/ 
        ssize_t n = recv(client_fd, buf, BUF_SIZE - 1, 0);
        if (n < 0) {
            perror("recv");
            break;
        }
        if (n == 0) {
            std::cout << "Client disconnected\n";
            break;
        }
        buf[n] = '\0';
        std::string received(buf, n);
        std::cout << "Received: " << received << '\n';

        // Echo back:
        ssize_t sent = send(client_fd, buf, n, 0);
        if (sent < 0) {
            perror("send");
            break;
        }
    }

    // close all PORTS once done
    close(client_fd);
    close(listen_fd);
}