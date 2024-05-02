#include "cgi.hpp"

CGI::CGI() {
    envirement = NULL;
    av = NULL;
    executable = "";

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
    int i = 0;
    if(req.getMethod()=="POST")
    {
        std::string script_filename = "SCRIPT_FILENAME=" + req.getPath();
        envirement[0] = strdup(script_filename.c_str());
        envirement[1] = strdup("REQUEST_METHOD=POST");
        std::string content_length = "CONTENT_LENGTH=" + req.headers["Content-Length"];
        envirement[2] = strdup(content_length.c_str());
        envirement[3] = strdup("REDIRECT_STATUS=200");
        std::string querystring ="QUERY_STRING="+ req.querystr;
        envirement[4] = strdup(querystring.c_str());
        envirement[5] = NULL;
        int pos = req.getPath().find_last_of('.');
        if(pos != std::string::npos) {
            executable = req.getPath().substr(pos + 1);
        }
    }
    else if(req.getMethod()=="GET")
    {
        std::string script_filename = "SCRIPT_FILENAME="+req.root_path+ req.getPath();
        envirement[0] = strdup(script_filename.c_str());
        envirement[1] = strdup("REQUEST_METHOD=GET");
        envirement[2] = strdup("REDIRECT_STATUS=200");
        std::string querystring ="QUERY_STRING="+ req.querystr;
        envirement[3] = strdup(querystring.c_str());
        envirement[4] = NULL;
        int pos = req.getPath().find_last_of('.');
        if(pos != std::string::npos) {
            executable = req.getPath().substr(pos + 1);
        }
    }
    else
    {
        req.status = 405;
    }
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
    int waitstatus;
    pid_t pid;
    int fdfile = open("cgifile.cgi", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    envirement_init(req);
    take_args(req);

    if((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    else if(pid == 0)
    {
        dup2(fdfile, 1);
        close(fdfile);
        if(execve(av[0], av, envirement) < 0)
        {
            perror("execve");
            exit(1);
        }
    }
    else
    {
        waitpid(pid, &waitstatus, 0);
        close(fdfile);
    }
}
