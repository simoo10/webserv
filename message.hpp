#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h> // Include epoll header
#include <cstring>
#include <sys/stat.h>
#include <climits>
#include <fcntl.h>
using namespace std;

const int MAX_CLIENTS = 5;
const int BUFFER_SIZE = 100000;



class Client {
    int clientSocket;
    sockaddr_in clientAddress;
    socklen_t clientAddressLength;
    char buffer[BUFFER_SIZE + 1];
    ssize_t bytesRead;
    char body[BUFFER_SIZE ];
    char *FullRequest;
    Client* next;
    public:
        Client(int& clientSocket, sockaddr_in& clientAddress, socklen_t& clientAddressLength) {
            this->clientSocket = clientSocket;
            this->clientAddress = clientAddress;
            this->clientAddressLength = clientAddressLength;
        }
            int getSocketDescriptor() const {
            return clientSocket;
        }
        const sockaddr_in& getClientAddress() const {
            return clientAddress;
        }

        char* getBuffer() {
            return buffer;
        }

        ssize_t getBytesRead() const {
            return bytesRead;
        }

        void setBytesRead(ssize_t bytesRead) {
            this->bytesRead = bytesRead;
        }

        Client* getNext() const {
            return next;
        }

        void setNext(Client* next) {
            this->next = next;
        }
};

class Responce {
    char *status;
    char *server;
    char *date;
    char *content_type;
    char *content_length;
    char *connection;
    char *body;
    char *FullResponce;
    public:
        Responce(char *FullResponce) {
            this->FullResponce = FullResponce;
        }
        void parseResponce() {
            char *token = strtok(FullResponce, "\n");
            while (token != NULL) {
                if (strstr(token, "HTTP/1.1") != NULL) {
                    status = token;
                } else if (strstr(token, "Server") != NULL) {
                    server = token;
                } else if (strstr(token, "Date") != NULL) {
                    date = token;
                } else if (strstr(token, "Content-Type") != NULL) {
                    content_type = token;
                } else if (strstr(token, "Content-Length") != NULL) {
                    content_length = token;
                } else if (strstr(token, "Connection") != NULL) {
                    connection = token;
                }
                token = strtok(NULL, "\n");
            }
        }
};

#endif