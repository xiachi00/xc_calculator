#include "httplogic.h"
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

namespace httplogic {

int ProcAccept(int client_fd, std::string &action,
    std::string &path, std::map<std::string, std::string> &args) {
    char buff[10240] = {0};
    int size = read(client_fd, buff, sizeof(buff));
    stringstream sstream(buff);
    string path_args_str,  key, value;
    sstream >> action >> path_args_str;
    cout<<"DEBUG:"<<action<<" "<<path_args_str<<endl;
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