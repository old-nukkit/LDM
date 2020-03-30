//
// Created by xubuntu on 3/28/20.
//
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <dlfcn.h>

using namespace std;

namespace HTTP {
    class Get {
    public:
        Get();

        virtual void start(string, const unordered_map<string, string> &, const unordered_map<string, string> &);

        virtual void setCallbacks(function<void (*)(float)> *, function<void (*)(int, int)> *);

        virtual vector<string> getProtocols();
    };
}

typedef HTTP::Get *(*create_t)();
typedef void (*destroy_t)(HTTP::Get *);

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage : " << argv[0] << " <URL> <MODULES>" << endl;
        return 1;
    }

    auto handle = dlopen(argv[2], RTLD_LAZY);
    if (!handle or handle == nullptr) {
        throw runtime_error("Can not open Http Module, Error desc: "s + dlerror());
    }

    auto create_class = (create_t) dlsym(handle, "create");
    auto destroy_class = (destroy_t) dlsym(handle, "destroy");

    string protocol = string(argv[1]).substr(0, string(argv[1]).find("://"));
    if (protocol.empty()) {
        cerr << "URL Doest not contain a protocol" << endl;
        return 1;
    }

    auto req = create_class();
    for (const string& pt : req->getProtocols()) {
        if (pt == protocol) {
            req->start(argv[1], {}, {});
            break;
        }
    }

    destroy_class(req);

    return 0;
}