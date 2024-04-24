#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include"request.hpp"
class request;
class Response {
    private:
        std::map<int, std::string> code;
    public:
        Response();
        void SendPostResponse(Request &req, int clientSocket);
};
#endif