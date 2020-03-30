#include "http.hpp"

// Get Method ------------------------------------------------------------------------
HTTP::Get::Get() {
    cout << "Class Get constructed" << endl;
}

void HTTP::Get::start(string url, const unordered_map<string, string> &header, const unordered_map<string, string> &params) {
    string protocol, hostname, port, path;
    _url = std::move(url);

    do {
        if (!getHeaderElement("Location").empty()) {
            parseUrl(getHeaderElement("Location"), protocol, hostname, port, path);
            closeConnection();
        } else {
            parseUrl(_url, protocol, hostname, port, path);
        }

        if (path[path.length() - 1] != '?')
            path += '?';

        for (const auto &key : params) {
            path += key.first;
            path += "=";
            path += key.second;
            path += "&";
        }
        path.erase(path.length() - 1);

        header_req = "GET "s + path + " HTTP/1.1" + "\r\n" +
                     "Host: " + hostname + "\r\n";

        for (const auto &key : header) {
            header_req += key.first;
            header_req += ": ";
            header_req += key.second;
            header_req += "\r\n";
        }

        header_req += "\r\n";
        sock = Socket(hostname, stoi(port), (protocol == "https"));
        sock.connect();
        sock.write(header_req.c_str(), header_req.length());

        getHeader();

        cout << header_resp << endl;
    } while (!getHeaderElement("Location").empty());
}

void HTTP::Get::setCallbacks(function<void (*)(float)> *progress_reporter, function<void (*)(int, int)> *download_progress) {
    prg_rprtr = progress_reporter;
    dl_prg = download_progress;
}

void HTTP::Get::parseUrl(string in, string &ptc, string &hn, string &prt, string &pth) {
	if (in[in.length() - 1] == '/')
		in.erase(in.length() - 1);

	if (in.find("//") != string::npos)
		ptc = in.substr(0, in.find("//") - 1);
	else {
		cerr << "URL Protocol not specified, defaulting to 'http://'" << endl;
		ptc = "http";
		in = ptc + "://" + in;
	}

	hn = u.split(u.split(in, "//")[1], "/")[0];
	string tmp = u.split(in, "//")[1];
	if (tmp.find('/') != string::npos)
		pth = tmp.substr(tmp.find('/'));
	else
		pth = "/";

	if (hn.find(':') != string::npos) {
		prt = u.split(hn, ":")[1];
		if (prt.empty()) {
			if (ptc == "http")
				prt = "80";
			else if (ptc == "https")
				prt = "443";
			else {
				throw runtime_error("This protocol (" + ptc + ") isn't supported yet.");
			}
		}
		hn.erase(hn.find(':'));
	} else {
		if (ptc == "http")
			prt = "80";
		else if (ptc == "https")
			prt = "443";
		else {
			throw runtime_error("This protocol (" + ptc + ") isn't supported yet.");
		}
	}
}

void HTTP::Get::getHeader() {
	char byte[1] = {0};
	vector<string> tmp;
	vector<char> resp_lines;
	string kv;
	header_resp.clear();
	resp_header.clear();
	tmp.clear();
	resp_lines.clear();
	resp_header_elmnts.clear();
	header_length = 0;

	while (true) {
		sock.read(byte, 1);
		resp_header.emplace_back(byte[0]);
		header_length++;
		if (byte[0] == '\r') {
			sock.read(byte, 1);
			resp_header.emplace_back(byte[0]);
			header_length++;
			if (byte[0] == '\n') {
				sock.read(byte, 1);
				resp_header.emplace_back(byte[0]);
				header_length++;
				if (byte[0] == '\r') {
					sock.read(byte, 1);
					resp_header.emplace_back(byte[0]);
					header_length++;
					if (byte[0] == '\n') {
						break;
					}
				}
			}
		}
	}

	header_resp = resp_header.data();
	u.replace(header_resp, "\r\n", "\n"); // All "\r\n" to "\n"
	u.replace(header_resp, "\n\n", "\n"); // One "\n\n" left from "\r\n\r\n" to "\n"

	kv.clear();
	for (auto c : header_resp) {
		if (c == '\n') {
			tmp.push_back(kv);
			kv.clear();
		} else {
			kv += c;
		}
	}

	string hrc = tmp[0];
	if (hrc.find("HTTP/1.1") != string::npos) {
		hrc = hrc.substr(hrc.find("HTTP/1.1") + string("HTTP/1.1 ").length(), 3);
		resp_code = stoi(hrc);
	}

	tmp.erase(tmp.begin());
	for (const string &line : tmp) {
		string first = u.split(line, ":")[0];
		transform(first.begin(), first.end(), first.begin(), ::tolower);
		string second = line.substr(line.find(": ") + 2);
		resp_header_elmnts[first] = second;
	}
}

void HTTP::Get::getFile(const string &path) {
    cout << "File ----------------------------------- " << path << endl;
}

string HTTP::Get::getHeaderElement(const string &elmnt) {
	string el = elmnt;
	transform(el.begin(), el.end(), el.begin(), ::tolower);
	return resp_header_elmnts[el];
}

void HTTP::Get::closeConnection() {
    sock.close();
}

vector<string> HTTP::Get::getProtocols() {
    return {"http", "https"};
}

HTTP::Get::~Get() {
    cout << "Get class destructed" << endl;
}