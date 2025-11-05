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

}