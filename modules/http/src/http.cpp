#include "http.hpp"

// Get Method ------------------------------------------------------------------------
HTTP::Get::Get(string url, const unordered_map<string, string> &header, const unordered_map<string, string> &params, string mp) : _url(std::move(url)) {
	string protocol, hostname, port, path;
	handler = dlopen(mp.c_str(), RTLD_LAZY);
	if (!handler or handler == nullptr) {
	    throw runtime_error("Cannot load library, handler : "s + dlerror());
	}

    auto creator = (creator_t ) dlsym(handler, "Socket_create");
    if (!creator or creator == nullptr) {
        throw runtime_error("Cannot load Socket_create");
    }

	do {
		if (!this->getHeaderElement("Location").empty()) {
			this->parseUrl(this->getHeaderElement("Location"), protocol, hostname, port, path);
			this->closeConnection();
		} else {
			this->parseUrl(_url, protocol, hostname, port, path);
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

		this->header_req = "GET "s + path + " HTTP/1.1" + "\r\n" +
			"Host: " + hostname + "\r\n";

		for (const auto &key : header) {
			this->header_req += key.first;
			this->header_req += ": ";
			this->header_req += key.second;
			this->header_req += "\r\n";
		}

		this->header_req += "\r\n";

        sock = (*creator)(hostname, stoi(port), (protocol == "https")); //this->sock = Socket(hostname, stoi(port), (protocol == "https"));
		this->sock->connect();
		this->sock->write(header_req.c_str(), header_req.length());

		this->getHeader();

		cout << header_resp << endl;
	} while (!this->getHeaderElement("Location").empty());
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
	this->header_resp.clear();
	this->resp_header.clear();
	tmp.clear();
	resp_lines.clear();
	this->resp_header_elmnts.clear();
	header_length = 0;

	while (true) {
		this->sock->read(byte, 1);
		this->resp_header.emplace_back(byte[0]);
		header_length++;
		if (byte[0] == '\r') {
			this->sock->read(byte, 1);
			this->resp_header.emplace_back(byte[0]);
			header_length++;
			if (byte[0] == '\n') {
				sock->read(byte, 1);
				this->resp_header.emplace_back(byte[0]);
				header_length++;
				if (byte[0] == '\r') {
					sock->read(byte, 1);
					this->resp_header.emplace_back(byte[0]);
					header_length++;
					if (byte[0] == '\n') {
						break;
					}
				}
			}
		}
	}

	this->header_resp = resp_header.data();
	u.replace(this->header_resp, "\r\n", "\n"); // All "\r\n" to "\n"
	u.replace(this->header_resp, "\n\n", "\n"); // One "\n\n" left from "\r\n\r\n" to "\n"

	kv.clear();
	for (auto c : this->header_resp) {
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
		this->resp_header_elmnts[first] = second;
	}
}

string HTTP::Get::getHeaderElement(const string &elmnt) {
	string el = elmnt;
	transform(el.begin(), el.end(), el.begin(), ::tolower);
	return this->resp_header_elmnts[el];
}

void HTTP::Get::closeConnection() {
    this->sock->close();
}