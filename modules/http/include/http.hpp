#ifndef LDM_HTTP_HPP
#define LDM_HTTP_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <thread>
#include <functional>

#include <fcntl.h>
#include <cerrno>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dlfcn.h>

#include "socket.hpp"
#include "utils.hpp"

using namespace std;

namespace HTTP {
    class Get {
    public:
        Get();

        ~Get();

        virtual void start(string, const unordered_map<string, string> &, const unordered_map<string, string> &);

        virtual void setCallbacks(function<void (*)(float)> *, function<void (*)(int, int)> *);

        virtual vector<string> getProtocols();

    private:
        Socket sock;
        Utils u;
        string _url, header_req, header_resp;
        unordered_map<string, string> resp_header_elmnts;
        vector<char> resp_header;
        int header_length{}, resp_code{};
        float downed = 0.0f;
        bool using_https = false, need_https = false;
        function<void (*)(float)> *prg_rprtr{};
        function<void (*)(int, int)> *dl_prg{};

        void getFile(const string &);

        void parseUrl(string, string &, string &, string &, string &);

        void getHeader();

        string getHeaderElement(const string &);

        void closeConnection();
    };
}

extern "C" {
    // u: Url
    // hp: Get Header Params
    // rp: Get Request params (param1=foo&param2=bar)
    HTTP::Get *create() {
        return new HTTP::Get();
    }

    void destroy(HTTP::Get *clz) {
        delete clz;
    }
};
#endif //LDM_HTTP_HPP
