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

//#include "../../poly.hpp"

using namespace std;

class Socket {
public:
    Socket(const std::string &, int, const bool &) {}

    ~Socket() {}

    virtual void connect() {}

    virtual void read(char *, int) {}

    virtual void write(const char *, int) {}

    virtual void close() {}
};

class MainSocket : public Socket {
public:
    MainSocket(const string &, int, const bool &);

    ~MainSocket();

    virtual void connect();

    virtual void read(char *, int);

    virtual void write(const char *, int);

    virtual void close();

private:
    struct addrinfo info{}, *result{};
    int sfd = -1;
    int error_code{};
    bool need_ssl = false;
    SSL_CTX *ssl_ctx{};
    SSL *ssl{};
};

extern "C" Socket *Socket_create(const string &h, int p, const bool &b) {
    return new MainSocket(h, p, b);
}

extern "C" void *Socket_destroy(Socket* s) {
    delete s;
    return nullptr;
}

#endif //LDM_SOCKET_HPP
