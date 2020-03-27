#include "socket.hpp"

MainSocket::~MainSocket() = default;

MainSocket::MainSocket(const string &host, int port, const bool &ns) : need_ssl(ns), Socket(host, port, need_ssl) {
    memset(&info, 0, sizeof(info));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;
    info.ai_protocol = IPPROTO_TCP;

    if (need_ssl) {
        SSL_load_error_strings();
        OpenSSL_add_ssl_algorithms();
    }

    int gai_res = getaddrinfo(host.c_str(), to_string(port).c_str(), &info, &result);
    if (gai_res != 0) {
        const char *error_desc = gai_strerror(gai_res);
        error_code = errno;
        throw runtime_error("getaddrinfo() failed, Error description : " + string(error_desc) +
                            "\n" +
                            "Error code : " + to_string(error_code));
    }

    sfd = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sfd == -1) {
        error_code = errno;
        throw runtime_error("socket() failed, Error code : " + to_string(error_code));
    }
}

void MainSocket::connect() {
    int conn_res = ::connect(sfd, result->ai_addr, result->ai_addrlen);
    if (conn_res == -1) {
        error_code = errno;
        throw runtime_error("connect() failed, Error code : " + to_string(error_code));
    }

    if (need_ssl) {
        ssl_ctx = SSL_CTX_new(TLS_client_method());
        if (!ssl_ctx or ssl_ctx == nullptr) {
            ERR_print_errors_fp(stderr);
            throw runtime_error("SSL_CTX_new() error, Cannot create SSL Context");
        }

        ssl = SSL_new(ssl_ctx);
        if (!ssl or ssl == nullptr) {
            ERR_print_errors_fp(stderr);
            throw runtime_error("SSL_new() error, Cannot create SSL");
        }

        int sslSetFd_res = SSL_set_fd(ssl, sfd);
        if (sslSetFd_res != 1) {
            ERR_print_errors_fp(stderr);
            throw runtime_error("SSL_set_fd() error, Cannot set file descriptor");
        }

        int sslConn_res = SSL_connect(ssl);
        if (sslConn_res != 1) {
            throw runtime_error("SSL_connect() error, Cannot perform a SSL/TLS Handshake"s +
                                "\n" +
                                "Error code : " + to_string(SSL_get_error(ssl, sslConn_res)));
        }
    }
}

void MainSocket::write(const char *data, int size) {
    size_t bl = size, bs = 0, b_sent = 0;
    if (need_ssl) {
        while ((b_sent = SSL_write(ssl, data + bs, bl)) != -1) {
            bl -= b_sent;
            bs += b_sent;

            if (bl == 0)
                break;
        }
    } else {
        while ((b_sent = ::send(sfd, data + bs, bl, 0)) != -1) {
            bl -= b_sent;
            bs += b_sent;

            if (bl == 0)
                break;
        }
    }
}

void MainSocket::read(char *data, int size) {
    size_t bl = size, br = 0, b_recv = 0;
    if (need_ssl) {
        while ((b_recv = SSL_read(ssl, data + br, bl)) != -1) {
            bl -= b_recv;
            br += b_recv;

            if (bl == 0)
                break;
        }
    } else {
        while ((b_recv = ::recv(sfd, data + br, bl, 0)) != -1) {
            bl -= b_recv;
            br += b_recv;

            if (bl == 0)
                break;
        }
    }
}

void MainSocket::close() {
    if (sfd < 0)
        return;

    if (need_ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }

    int close_res = ::close(sfd);
    if (close_res == -1) {
        error_code = errno;
        throw runtime_error("close() failed, Error code : " + to_string(error_code));
    }

    if (need_ssl) {
        SSL_CTX_free(ssl_ctx);
        EVP_cleanup();
    }
}
