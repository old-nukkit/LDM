#include "core.hpp"

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

Core::Core(const string &uri, const string &modules_dir) {
    // Initiate Supported URI Schemes and Their module path
    schemes["http"] = getLibraryPath(modules_dir, "ldm_module-http");
    schemes["https"] = getLibraryPath(modules_dir, "ldm_module-http");

    int colon_pos = uri.find(':');
    if (colon_pos == string::npos) {
        throw runtime_error("URI : Bad syntax");
    }

    string scheme = uri.substr(0, colon_pos);
    if (scheme.empty()) {
        throw runtime_error("URI : Scheme not defined");
    }

    if (schemes[scheme].empty()) {
        throw runtime_error("URI : Scheme not supported");
    }

    auto handle = dlopen(schemes[scheme].c_str(), RTLD_LAZY);
    if (!handle or handle == nullptr) {
        throw runtime_error("Can not open Http Module, Error desc: "s + dlerror());
    }

    auto create_class = (create_t) dlsym(handle, "create");
    auto destroy_class = (destroy_t) dlsym(handle, "destroy");

    auto req = create_class();
    for (const string &pt : req->getProtocols()) {
        if (pt == scheme) {
            req->start(uri, {}, {});
            break;
        }
    }

    destroy_class(req);
}

inline string Core::getLibraryPath(const string &md_dir, const string &lib_name) {
    string dir = md_dir;
    if (!dir.ends_with('/')) dir.append("/");

    return dir + library_prefix + lib_name + library_suffix;
}
