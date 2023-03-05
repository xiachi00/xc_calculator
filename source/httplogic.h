#include <string>
#include <map>


namespace httplogic {


int ProcAccept(int client_fd, std::string &action,
    std::string &path, std::map<std::string, std::string> &args);



};
