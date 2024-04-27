#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include"request.hpp"
class request;
class Response {
    private:
        std::map<int, std::string> code;
    public:
        Request *req;
        Response(Request *req);
        void SendPostResponse(int clientSocket);
};
#endif