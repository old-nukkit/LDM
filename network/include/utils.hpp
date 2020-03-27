#ifndef LDM_UTILS_HPP
#define LDM_UTILS_HPP

#include <string>
#include <vector>
#include <iomanip>

using namespace std;

#define debug(msg) \
    cout << (msg) << endl;

class Utils {
public:
    vector<string> split(const string &str, const string &del) {
        if (str.empty())
            return {};

        string _str = str;
        vector<string> tokens;
        size_t pos = 0;
        while ((pos = _str.find(del)) != string::npos) {
            tokens.emplace_back(_str.substr(0, pos));
            _str.erase(0, pos + del.length());
        }
        tokens.emplace_back(_str);

        return tokens;
    }

    bool replace(string &str, const string &from, const string &to) {
        if (str.empty())
            return false;

        size_t pos = 0;
        while ((pos = str.find(from, pos)) != string::npos) {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }

        return true;
    }
};

class ProgressBar {
private:
    int overhead = " [100%]"s.length();
    ostream &os;
    const size_t bar_width;
    string message;
    const string full_bar;

public:
    ProgressBar(ostream &os, size_t line_width, string message_, const char symbol = '.') : os{os},
        bar_width{line_width - overhead}, message{move(message_)},
        full_bar{string(bar_width, symbol) + string(bar_width, ' ')} {
        if (message.length() + 1 >= bar_width || message.find('\n') != message.npos) {
            os << message << '\n';
            message.clear();
        }
        write(0.0);
    }

    ~ProgressBar() {
        os << '\n';
    }

    void write(double fraction) {
        if (fraction < 0)
            fraction = 0;
        else if (fraction > 1)
            fraction = 1;

        auto width = bar_width - message.size();
        auto offset = bar_width - static_cast<unsigned>(width * fraction);

        os << '\r' << message;
        os.write(full_bar.data() + offset, width);
        os << "[" << static_cast<int>(100 * fraction) << "%]" << flush;
    }
};

#endif
