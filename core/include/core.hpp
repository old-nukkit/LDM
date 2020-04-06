#ifndef LDM_CORE_HPP
#define LDM_CORE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>

#include <dlfcn.h>

#include "utils.hpp"
using namespace std;

class Core {
public:
    explicit Core(const string &, const string &);

private:
    map<string, string> schemes;
    string library_prefix = "lib", library_suffix = ".so";

    inline string getLibraryPath(const string &, const string &);
};


#endif //LDM_CORE_HPP
