#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include"request.hpp"

class Response {
    private:
        std::map<int, std::string> code;
    public:
        Response();
        void sendResponse(int clientsocket);
};