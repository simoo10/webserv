#include "cgi.hpp"

CGI::CGI() {
    envirement = NULL;
    av = NULL;
    executable = "";

}

std::string change_the_key(const std::string& str) {
    std::string upperkey = str;
    for (size_t i = 0; i < upperkey.length(); ++i) {
        if (upperkey[i] == '-') {
            upperkey[i] = '_';
            continue;
        }
        upperkey[i] = std::toupper(upperkey[i]);
        
    }
    return(upperkey);
}

// void CGI::print_req_headers(Request &req) {
//     std::map<std::string, std::string>::iterator it;
//     for(it = req.headers.begin(); it != req.headers.end(); it++)
//     {
//         std::cout << it->first << " : " << it->second << std::endl;
//     }
// }

CGI::~CGI() {
}


void CGI:: envirement_init(Request &req) {
    envirement = new char*[4];
    size_t i = 0;
    std::map<std::string, std::string>::const_iterator it;
    std:: string methode = "REQUEST_METHOD="+req.getMethod();
    std::string root = "home/met-tahe/Desktop/webserv";
    std::string script_filename ="SCRIPT_FILENAME="+req.getPath();
    std::string REDIRECT_STATUS = "REDIRECT_STATUS=200";
    executable = script_filename.substr(script_filename.find_last_of('.') + 1);
    envirement[i] = strdup(script_filename.c_str());
    i++;
    envirement[i] = strdup(methode.c_str());
    i++;
    envirement[i] = strdup(REDIRECT_STATUS.c_str());
    i++;
    for (it = req.headers.begin(); it != req.headers.end(); it++) {
        std::string key = change_the_key(it->first);
        std::string value = it->second;
        std::string env = key + "=" + value;
    std::cout<<"==========="<<std::endl;
        envirement[i] = strdup(env.c_str());
        i++;
    }
    envirement[i] = NULL;
}

void CGI::take_args(Request &req) {
    av = new char*[3];

    std::map<std::string, std::string> cgi_extension;
    cgi_extension["php"] = "/usr/bin/php-cgi";
    cgi_extension["py"] = "/usr/bin/python3";
    cgi_extension["pl"] = "/usr/bin/perl";
    cgi_extension["rb"] = "/usr/bin/ruby";
    cgi_extension["sh"] = "/bin/sh";
    std::map<std::string, std::string>::iterator it = cgi_extension.find(executable);
    if(it != cgi_extension.end()) {
        av[0] = strdup(it->second.c_str());
        av[1] = strdup(envirement[0] + 16);
        av[2] = NULL;
        std::cout<<"->"<<av[0]<<std::endl;
        std::cout<<"->"<<av[1]<<std::endl;
    }
    else {
        throw std::runtime_error("CGI: invalid extension");
    }
}

void CGI::execute_cgi(Request &req)
{
    int status;
    pid_t pid;
    // std::map<std::string, std::string>::iterator it;
    // for(it = req.getHeaders().begin(); it != req.getHeaders().end(); it++)
    // {
    //     std::cout<<it->first<<" "<<it->second<<std::endl;
    // }
    envirement_init(req);
    take_args(req);

    std::cout<<"=>"<<av[0]<<std::endl;
    std::cout<<"=>"<<av[1]<<std::endl;
    int fd[2];
    if (pipe(fd) == -1)
        throw std::runtime_error("CGI: pipe failed");
    std::cout<<"===========>>>>>"<<std::endl;
    if ((pid = fork()) == -1)
        throw std::runtime_error("CGI: fork failed");
    if (pid == 0) {
    if (execve(av[0], av, envirement) == -1)
    {
            throw std::runtime_error("CGI: execve failed");
    }
    }
    else {
        waitpid(pid, &status, 0);
    }

}