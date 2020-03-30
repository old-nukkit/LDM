#ifndef LDM_SOCKET_HPP
#define LDM_SOCKET_HPP

#include <iostream>
#include <string>

#include <cstdlib>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <netdb.h>
#include <unistd.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
using namespace std;

class Socket {
public:
    Socket() = default;

    Socket(const string &, int, const bool &);

    ~Socket();

    void connect();

    void read(char *, int);

    void write(const char *, int);

    void close();

private:
    struct addrinfo info{}, *result{};
    int sfd = -1;
    int error_code{};
    bool need_ssl = false;
    SSL_CTX *ssl_ctx{};
    SSL *ssl{};
};

#endif //LDM_SOCKET_HPP