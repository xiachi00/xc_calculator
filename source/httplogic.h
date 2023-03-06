#include <string>
#include <unordered_map>
#include <ostream>


class httpRequest {
public:
    std::string method;                          // 请求方法
    std::string url;                             // 要请求的URL地址
    std::unordered_map<std::string, std::string> args;     // 请求参数
    std::string version;                         // 版本协议号
    std::unordered_map<std::string, std::string> headers;  // 请求头
    friend std::ostream & operator << (std::ostream& os, const httpRequest &obj);
};

class httpResponse {
    
};

namespace httplogic {

    int Request(int client_fd, httpRequest &req);
    int Response(int client_fd, httpResponse &rsp);

};
