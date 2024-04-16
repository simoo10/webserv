#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include<cstring>
#include"message.hpp"
class Request{
    private:
        std::string method;
        std::string path;
        char buffer[BUFFER_SIZE + 1];
        std::string version;
        std::map<std::string, std::string> headers;
        std::string body;
        int status;
        bool header_status;
    public:
        Client *current;
        Request();
        Request(std::string method, std::string path, std::string version, std::map<std::string, std::string> headers, std::string body);
        std::string getMethod();
        std::string getPath();
        std::string getVersion();
        std::map<std::string, std::string> getHeaders();
        std::string getBody();
        ~Request();
        int recieveRequest(int clientSocket);
        void takeRequest(std::string filename);
        int parseheaders(std::string headers);
        int parseRequest(char *req,int bytesRead);
        void method_handler(std::string method,char *body);
        void post_handler(char *body);
        std::string content_type_handler();
        void chunked_request_handler(char *bd);
        void request_status_code();
};

#endif