#ifndef CGI_HPP
#define CGI_HPP

#include"request.hpp"
#include<sys/wait.h>
class Request;
class CGI {
    private:
        // std::vector<std::string> env;
        char **envirement;
        std::string executable;
        char **av;
    public:
        CGI();
        ~CGI();
        void envirement_init(Request &req);
        void take_args(Request &req);
        void execute_cgi(Request &req);
        void print_req_headers(Request &req);
};

#endif