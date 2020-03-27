#ifndef LDM_HTTP_HPP
#define LDM_HTTP_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <thread>

#include <fcntl.h>
#include <cerrno>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dlfcn.h>

//#include "socket.hpp"
//#include "../../../poly.hpp"
#include "utils.hpp"

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

namespace HTTP {
    class Get {
    public:
        Get(string, const unordered_map <string, string> &, const unordered_map <string, string> &, string mp);

    private:
        //Socket sock;
        Utils u;
        string _url, header_req, header_resp;
        unordered_map <string, string> resp_header_elmnts;
        vector<char> resp_header;
        int header_length{}, resp_code{};
        float downed = 0.0f;
        bool using_https = false, need_https = false;

        void *handler = nullptr;
        typedef Socket *(*creator_t)(const string &, int, const bool &);
        Socket *sock;

        void parseUrl(string, string &, string &, string &, string &);

        void getHeader();

        string getHeaderElement(const string &);

        void closeConnection();
    };
}
#endif //LDM_HTTP_HPP
