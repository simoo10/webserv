#include "message.hpp"
#include"request.hpp"
#include"config.hpp"
#include"response.hpp"

int main(int ac,char **av) {
    int j = 0;
   std::map < std::string, serverConfig >::iterator it_servers;
    std::vector<int> serverSockets;
    std::vector<int>::iterator it;
    std::vector<Client> Clients;
    int serverSocket,epollFd;
    epoll_event event;
    Request req;
    Response res(&req);
    //char buff[BUFFER_SIZE];
    //epoll_event events[MAX_CLIENTS];
    signal(SIGPIPE, SIG_IGN);
    if(ac != 2)
           throw std::runtime_error("./webserv [configuration file]");
        GlobalConfig config(av[1]);
        for(it_servers = config.servers.begin();it_servers !=config.servers.end() ; it_servers++ )
        {
            for(size_t i = 0 ;i < it_servers->second.config["listen"].size() ; i++)
            {
                serverSocket = socket(AF_INET,SOCK_STREAM,0);
                if(serverSocket == -1)
                    throw std::runtime_error("Failed to create socket.");
                serverSockets.push_back(serverSocket);
                sockaddr_in serverAddress;
                memset(&serverAddress,0,sizeof(serverAddress));
                serverAddress.sin_family = AF_INET;
                std::cout<<"---->"<<atoi(it_servers->second.config["listen"].at(i).c_str())<<std::endl;
                serverAddress.sin_port = htons(atoi(it_servers->second.config["listen"].at(i).c_str()));
                serverAddress.sin_addr.s_addr = inet_addr(it_servers->first.c_str());
                int on = 1;
                if (setsockopt(serverSocket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) == -1) {
                std::cerr << "Error: setsockopt failed" << std::endl;
                continue;
                }
                // Bind socket to address and port
                if(bind(serverSocket,(sockaddr*)&serverAddress,sizeof(serverAddress)) == -1)
                    throw std::runtime_error("Failed to bind socket.");
                // Listen for incoming connections
                if(listen(serverSocket,5) == -1)
                    throw std::runtime_error("Failed to listen.");
                std::cout << "Server started. Listening on port :" << it_servers->second.config["listen"].at(i) << std::endl;
             }
        }

    // Add server socket to epoll
       epollFd = epoll_create(1);
       if(epollFd == -1)
            throw std::runtime_error("Failed to create epoll instance.");
        std::vector<epoll_event> serverEvents(serverSockets.size());
        for(size_t i = 0 ; i < serverSockets.size() ; i++)
        {
            serverEvents[i].data.fd = serverSockets.at(i);
            serverEvents[i].events = EPOLLIN;
            if(epoll_ctl(epollFd,EPOLL_CTL_ADD,serverSockets[i],&serverEvents[i]) == -1)
                throw std::runtime_error("Failed to add server socket to epoll instance.");
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
                req.clientSocket = clientSocket;
                std::cout<<"+++++++++++"<<clientSocket<<std::endl;
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
                      //cout << "im here" << endl;
                        ssize_t bytesRead = recv(fd, current->getBuffer(), BUFFER_SIZE, 0);
                        //std::cout<<current->getBuffer()<<std::endl;
                        if (bytesRead <= 0) {
                            // Connection closed or error
                            if (bytesRead == 0)
                                std::cout << "Client " << fd << " disconnected.\n";
                            else 
                                perror("Error receiving data");
                            close(fd);
                        } 
                        else {
                            // Process received data
                            current->getBuffer()[bytesRead] = '\0';
                            current->setBytesRead(bytesRead);
                           // std::cout << "Received -------------------------------------------.>" << bytesRead << std::endl;
                            // reqstr.append(current->getBuffer(), bytesRead);
                            // if(reqstr.find("\r\n\r\n") == std::string::npos)
                            // {
                            //     //std::cout<<"===+++++++++++++=================="<<std::endl;
                            //     current = current->getNext();
                            //     continue;
                            // }
                            
                            if(req.parseRequest(current->getBuffer(),bytesRead,config) == 1)
                            {
                                current = current->getNext();
                                continue;
                            }
                            // if(req.request_status == true)
                            // {
                            //     res.SendPostResponse(fd);
                            //     req.request_status = false;
                            //     current = current->getNext();
                            //     continue;
                            // }
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
