#include <string>
#include <map>


namespace httplogic {


int ProcAccept(int client_fd, std::string &action,
    std::string &path, std::map<std::string, std::string> &args,
    std::string &http_ver, std::map<std::string, std::string> &headers);

int ProcResponse(int client_fd, const char *wwwroot, const std::string &action,
    const std::string &path, std::map<std::string, std::string> &args,
    std::string &http_ver, std::map<std::string, std::string> &headers);
};
