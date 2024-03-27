#include "message.hpp"
#include"request.hpp"

int main() {
    int j = 0;
    Request req;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        return EXIT_FAILURE;
    }

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error binding socket");
        close(serverSocket);
        return EXIT_FAILURE;
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Error listening on socket");
        close(serverSocket);
        return EXIT_FAILURE;
    }

    std::cout << "Server is listening on port 8080...\n";

    int epollFd = epoll_create1(0);
    if (epollFd == -1) {
        perror("Error creating epoll instance");
        close(serverSocket);
        return EXIT_FAILURE;
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serverSocket;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
        perror("Error adding server socket to epoll instance");
        close(serverSocket);
        close(epollFd);
        return EXIT_FAILURE;
    }

    std::vector<struct epoll_event> events(MAX_CLIENTS + 1); // Events array for epoll_wait

    Client* head = nullptr; // Head of the linked list

    while (true) {
        int numEvents = epoll_wait(epollFd, events.data(), MAX_CLIENTS + 1, -1);
        if (numEvents == -1) {
            perror("Error in epoll_wait");
            break;
        }

        for (int i = 0; i < numEvents; ++i) {
            int fd = events[i].data.fd;

            if (fd == serverSocket) {
                // New connection
                int clientSocket = accept(serverSocket, nullptr, nullptr);
                if (clientSocket == -1) {
                    perror("Error accepting connection");
                } else {
                    std::cout << "New connection accepted. Client socket: " << clientSocket << std::endl;

                    // Create a new client object
                    sockaddr_in clientAddress;
                    socklen_t clientAddressLen = sizeof(clientAddress);
                    getpeername(clientSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
                    Client* newClient = new Client(clientSocket, clientAddress, clientAddressLen);

                    // Add the new client to the linked list
                    newClient->setNext(head);
                    head = newClient;

                    // Add the new client socket to the epoll instance
                    event.events = EPOLLIN;
                    event.data.fd = clientSocket;
                    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
                        perror("Error adding client socket to epoll instance");
                        close(clientSocket);
                    }
                }
            } else {
                // Data received on a client socket
                Client* current = head;
                while (current != nullptr) {
                    if (current->getSocketDescriptor() == fd) {
                       // vector<char> buffer(BUFFER_SIZE);
                        ssize_t bytesRead = recv(fd, current->getBuffer(), BUFFER_SIZE, 0);
                        std::cout<<"jjjjjjjjj"<<std::endl;
                        //cout<<"------------"<<endl;
                        //cout << current->getBuffer() << endl;
                        //cout << "bytesRead = " << bytesRead << endl;
                        if (bytesRead <= 0) {
                            // Connection closed or error
                            if (bytesRead == 0) {
                                std::cout << "Client " << fd << " disconnected.\n";
                            } else {
                                perror("Error receiving data");
                            }
                            close(fd);
                        } else {
                            // Process received data
                            current->getBuffer()[bytesRead] = '\0';
                            current->setBytesRead(bytesRead);
                            std::cout<<current->getBuffer()<<std::endl;
                            std::string reqstr(current->getBuffer(),bytesRead);
                            req.parseRequest(reqstr);
                            req.method_handler(req.getMethod());
                        }
                        break;
                    }
                    current = current->getNext();
                }
            }
        }
    }

    // Close all client sockets and delete client objects
    Client* current = head;
    while (current != nullptr) {
        close(current->getSocketDescriptor());
        Client* next = current->getNext();
        delete current;
        current = next;
    }

    // Close the server socket
    close(serverSocket);
    close(epollFd);

    return 0;
}
