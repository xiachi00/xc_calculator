#include "httplogic.h"
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <ostream>
using namespace std;

ostream & operator << (ostream& os, const httpRequest &obj) {
    os << "Method: " << obj.method << endl;
    os <<  "Url: " << obj.url << endl;
    os << "Version: " << obj.version << endl;
    os << "Args: " << endl;
    for (auto &p : obj.args) {
        os << "     " << p.first << ": " << p.second << endl;
    }
    os << "Headers: " << endl;
    for (auto &p : obj.headers) {
        os << "     " << p.first << ": " << p.second << endl;
    }
    return os;
}

namespace httplogic {

void Format(string &s) {
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.pop_back();
    if (s.empty()) return;
    size_t pos = 0;
    while (pos < s.size() && (s[pos] == '\n' || s[pos] == '\r')) pos += 1;
    s = s.substr(pos);
}

vector<string> split(string &str, char c) { // c 为分隔符
    stringstream ss(str);
    vector<string> res;
    string item;
    while (getline(ss, item, c)) res.push_back(item);
    return res;
}

int Request(int client_fd, httpRequest &req) {
    char buff[10240] = {0};
    int len = read(client_fd, buff, sizeof buff);
    if (len < 0) return -1;
    stringstream sline(buff);
    string line;
    
    //  处理请求行
    getline(sline, line), Format(line);
    vector<string> first_line = split(line, ' ');

    req.method = first_line[0];                                // method
    req.version = first_line[2];                               // version
    vector<string> url_args = split(first_line[1], '?');
    req.url = url_args[0];                                     // url
    vector<string> args = split(url_args[1], '&');
    vector<string> key_value;                                  
    for (auto &item : args) {
        key_value = split(item, '=');
        key_value.resize(2);
        req.args[key_value[0]] = key_value[1];                 // args[key] = value
    }

    //  处理请求头
    while (getline(sline, line)) {
        Format(line);
        key_value = split(line, ':');
        key_value.resize(2);
        req.headers[key_value[0]] = key_value[1];              // headers[key] = value
    }
    cout << req << endl;
}

int Response(int client_fd, httpResponse& rsp) {
    
}






}
