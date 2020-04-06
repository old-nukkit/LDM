#include <iostream>

#include "core.hpp"
using namespace std;

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Usage : " << argv[0] << " <URL> [MODULES DIR]" << endl;
        return 1;
    }

    Core core(argv[1], (argc == 3) ? argv[2] : "/usr/lib");

    return 0;
}