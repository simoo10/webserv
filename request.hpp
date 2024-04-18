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
#include <algorithm>
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
        int bodylength;
        int bytes_read;
        bool file_status;
        bool chunked_status;
        bool flag;
        long chunksize;
        int iposition;
    public:
        long  content_length;
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
        void method_handler(std::string method,char *body, int i);
        void post_handler(char *body, int i);
        std::string content_type_handler();
        void chunked_request_handler(char *body,int i);
        void post_handler1(std::string body);
        void request_status_code();
};

#endif