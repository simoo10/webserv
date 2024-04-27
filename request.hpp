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
#include"config.hpp"
#include"cgi.hpp"
using namespace std;

class GlobalConfig;
class CGI;
class Request{
    private:
        std::string method;
        std::string path;
        char buffer[BUFFER_SIZE];
        std::string version;
        std::string body;
        
        bool header_status;
        int bodylength;
        int bytes_read;
        bool file_status;
        bool chunked_status;
        bool flag;
        long chunksize;
        bool check;
        int iposition;
        bool hexa_status;
    public:
        std::map<std::string, std::string> headers;
    int status;
        int clientSocket;
        string filename;
        long  content_length;
        Client *current;
        bool request_status;
        //GlobalConfig config;
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
        int parseheaders(std::string headers,GlobalConfig &config);
        int parseRequest(char *req,int bytesRead,GlobalConfig &config);
        void method_handler(std::string method,char *body, int i);
        void post_handler(char *body, int i);
        std::string content_type_handler();
        void chunked_request_handler(char *body,int i);
        void post_handler1(std::string body);
        void request_status_code();
};

#endif