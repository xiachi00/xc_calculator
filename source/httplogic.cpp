#include "httplogic.h"
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

namespace httplogic {

int ProcAccept(int client_fd, std::string &action,
    std::string &path, std::map<std::string, std::string> &args,
    std::string &http_ver, std::map<std::string, std::string> &headers) {
    char buff[10240] = {0};
    int size = read(client_fd, buff, sizeof(buff));
    stringstream sstream(buff);
    // 1.响应行  "GET /path?arg1=23&arg2=44 HTTP/1.1"
    string path_args_str, key, value;
    sstream >> action >> path_args_str >> http_ver;
    cout<<"DEBUG:"<<action<<" "<<path_args_str << " " << http_ver <<endl;
    // cout<<"BUFF:"<<buff<<endl;
    path = "";
    args.clear();
    size_t pos = path_args_str.find('?'), pos2;
    if (pos != string::npos) {
        path = path_args_str.substr(0, pos);
        while(pos != string::npos && pos < path_args_str.size()) {
            pos2 = path_args_str.find('=', pos);
            if (pos2 == string::npos) continue;
            key = path_args_str.substr(pos + 1, pos2 - pos - 1);
            pos = path_args_str.find('&', pos2);
            if (pos == string::npos) pos = path_args_str.size();
            value = path_args_str.substr(pos2 + 1, pos - pos2 - 1);
            args[key] = value;
        }
    } else {
        path = path_args_str;
    }
    // 2.响应头   “Connection: keep-alive\r\nCache-Control: max-age=0\r\n”
    string line;
    while(getline(sstream, line) && line.size()) {
        while(line.size() && line.back() == '\r') line.pop_back();
        if (line.size() == 0) continue;
        pos = line.find(':');
        key = line.substr(0, pos);
        value = line.substr(pos + 2);
        headers[key] = value;
    }
    return 0;
}

int ProcResponse(int client_fd, const char *wwwroot, const std::string &action,
        const std::string &path, std::map<std::string, std::string> &args,
        std::string &http_ver, std::map<std::string, std::string> &headers) {
    // 1.响应行
    string message = http_ver + " 200 OK";
    // 2.响应头
    if (headers["Accept"].find("text/html") != string::npos) {
        if (path == "/" || path.size() == 0) {
            path = "index.html";
        }
        
    }
    /*

    message="HTTP/1.1 200 OK\r\n";                                    //响应行
    message+="Content-Type:text/html\r\n";                             //响应头
    message+="\r\n";                                                   //空行
    
    ifstream ifile;
    ifile.open(file_name, ios::in);
    string jh;
    while(getline(ifile,jh))
    {
        message += jh + "\n";
    }
    ifile.close();
    message += "\r\n";



    write(client_fd, message.c_str(), message.size());//返回message
    */
    return 0;
}

};

// int main() {
//     std::string path_args_str("/path?arg1=23&arg2=44&arg3=&arg4=dsad");
//     cout << path_args_str << endl;
//     std::string path = "";
//     std::map<std::string, std::string> args;
//     size_t pos = path_args_str.find('?');
//     size_t pos2;
//     std::string key, value;
//     if (pos != string::npos) {
//         path = path_args_str.substr(0, pos);
//         while(pos != string::npos && pos < path_args_str.size()) {
//             pos2 = path_args_str.find('=', pos);
//             if (pos2 == string::npos) continue;
//             key = path_args_str.substr(pos + 1, pos2 - pos - 1);
//             pos = path_args_str.find('&', pos2);
//             if (pos == string::npos) pos = path_args_str.size();
//             value = path_args_str.substr(pos2 + 1, pos - pos2 - 1);
//             args[key] = value;
//         }
//     } else {
//         cout<<"66"<<endl;
//     }

//     return 0;
// }