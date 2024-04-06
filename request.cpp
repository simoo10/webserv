#include"request.hpp"
#include<unistd.h>
Request::Request(){
    this->method = "";
    this->path = "";
    this->version = "";
    this->headers = std::map<std::string, std::string>();
    this->body = "";
}

Request::Request(std::string method, std::string path, std::string version, std::map<std::string, std::string> headers, std::string body){
    this->method = method;
    this->path = path;
    this->version = version;
    this->headers = headers;
    this->body = body;
}

std::string Request::getMethod(){
    return this->method;
}

std::string Request::getPath(){
    return this->path;
}

std::string Request::getVersion(){
    return this->version;
}

std::map<std::string, std::string> Request::getHeaders(){
    return this->headers;
}

std::string Request::getBody(){
    return this->body;
}

Request::~Request(){
    this->method = "";
    this->path = "";
    this->version = "";
    this->headers.clear();
    this->body = "";
}

// int Request::recieveRequest(int fd){
//     std::string reqstr;
//     ssize_t bytesRead = recv(fd, buffer, BUFFER_SIZE, 0);
//     std::cout<<"bytesRead===>"<<bytesRead<<std::endl; 
//     //std::cout<<buffer<<std::endl;
//     if (bytesRead <= 0) {
//         if (bytesRead == 0)
//             std::cout << "Client " << fd << " disconnected.\n";
//         else 
//             perror("Error receiving data");
//         close(fd);
//     } 
//     else {
        
//         buffer[bytesRead] = '\0';std::cout<<"pppppppppppppp"<<std::endl;
//        // current->setBytesRead(bytesRead);
//         std::cout<<"qqqqqqqqqqqqqqqqqq"<<std::endl;
//         std::cout << "Received -------------------------------------------.>" << bytesRead << std::endl;
//         reqstr.append(buffer, bytesRead);
//          if(reqstr.find("\r\n\r\n") == std::string::npos)
//         {
//             std::cout<<"===+++++++++++++=================="<<std::endl;
//             return(1);
//         }
//         std::cout<<"----------------------"<<std::endl;
//         std::cout<<reqstr<<std::endl;
//         parseRequest(reqstr);
//     }
//     return(0);
// }


void Request::takeRequest(std::string filename)
{
    std::ifstream i;
    i.open(filename.c_str());
    std::string req;
    std::string line;
    while(getline(i, line))
    {
        req += line;
        req += "\r\n";
    }
    i.close();
    parseRequest(req);
}

void Request::parseRequest(std::string req)
{
    int i = 0;
    int pip = 0;
    pip = req.find("\r\n");
    std::cout<<"req===>"<<req<<std::endl; 
    std::string reqline = req.substr(0, pip);
    while(reqline[i])
    {
        if(reqline[i] == ' ')
        {
            std::cout<<"-------------------------"<<std::endl;
            this->method = reqline.substr(0, i);
            std::cout<<"====================="<<std::endl;
            break;
        }
        i++;
    }
    
    int j = i+1;
    
    while(reqline[j])
    {
        if(reqline[j] == ' ')
        {
            //std::cout<<"-------------------------"<<std::endl;
            this->path = reqline.substr(i+1, j-i-1);
            //std::cout<<"====================="<<std::endl;
            break;
        }
        j++;
    }
    std::cout<<"reqline.length()-j-1==="<<reqline.length()<<std::endl;
    std::cout<<"j+1==="<<j+1<<std::endl;
    this->version = reqline.substr(j+1, reqline.length()-j-1);std::cout<<"====================="<<std::endl;
    i = pip+2;
    while(req[i] != '\r' && req[i+1] != '\n')
    {
        int k = req.find(":", i);
        std::string key = req.substr(i, k-i);
        int l = req.find("\r\n", k);
        std::string value = req.substr(k+2, l-k-2);
        this->headers[key] = value;
        i = l+2;
    }
    i += 2;
    std::cout<<"===>"<<path<<std::endl;
    if(i < req.length())
    {
        if(headers["Transfer-Encoding"]=="chunked")
        {
            //std::cout<<"=============aaaaaaaaaaa========="<<std::endl;
           // exit(0);
            chunked_request_handler(req.substr(i, req.length()-i));
            
        }
        else
            this->body = req.substr(i, req.length()-i);
    }
    method_handler(this->method);
}

void Request::method_handler(std::string method)
{
    if(method == "GET")
    {
        //
    }
    else if(method == "DELETE")
    {
        //
    }
    else if(method == "POST")
    {
        // if(headers["Transfer-Encoding"]=="chunked")
        // {
        //     //std::cout<<"=============aaaaaaaaaaa========="<<std::endl;
        //    // exit(0);
        //    // chunked_request_handler();
            
        // }
        else
        {
        //     std::cout<<"aaaaaaaaaaaaaaaa"<<std::endl;
            post_handler(this->body);
        // }
    }
    else
    {
        std::cout<<"Invalid Method"<<std::endl;
    }
}

std::string Request::content_type_handler()
{
    std::map<std::string, std::string>::iterator it;
    std::map<std::string,std::string>contenttype;
    contenttype["application/json"] = ".json";
    contenttype["text/css"] = ".css";
    contenttype["image/gif"] = ".gif";
    contenttype["text/csv"] = ".csv";
    contenttype["text/html"] = ".html";
    contenttype["image/jpeg"] = ".jpeg";
    contenttype["text/javascript"] = ".js";
    contenttype["image/png"] = ".png";
    contenttype["text/plain"] = ".txt";
    contenttype["image/svg+xml"] = ".svg";
    contenttype["application/pdf"] = ".pdf";
    contenttype["image/x-icon"] = ".ico";
    contenttype["video/mp4"] = ".mp4";

    for(it = this->headers.begin(); it != this->headers.end(); it++)
    {
        if(it->first == "Content-Type")
            return(contenttype[it->second]);
    }
    return (0);
}

void Request::post_handler(std::string body)
{
    std::ofstream o;
    if(body.empty())
        return;

    std::string filename = "post" + content_type_handler();
	o.open(filename.c_str());
    o << body;
    o.close();
}
int hexatoint(string hex)
{
    int chunksize = 0;
    for(int i = 0; i < hex.length(); i++)
    {
        if(hex[i] >= '0' && hex[i] <= '9')
            chunksize = chunksize*16 + hex[i] - '0';
        else if(hex[i] >= 'A' && hex[i] <= 'F')
            chunksize = chunksize*16 + hex[i] - 'A' + 10;
        else if(hex[i] >= 'a' && hex[i] <= 'f')
            chunksize = chunksize*16 + hex[i] - 'a' + 10;
    }
    return (chunksize);

}

void Request::chunked_request_handler(std::string bd)
{
    std::string chunk;
    int i = 0;
    while(i<bd.length())
    {
        int pip = bd.find("\r\n", i);
        int chunksize = hexatoint(bd.substr(i, pip-i));
        if(chunksize == 0)
            break;
        i = pip+2;
        chunk.append(bd.substr(i, chunksize));
        i += chunksize+2;
    }
    post_handler(chunk);
}



// int main()
// {
//     Request req;
//     req.parseRequest("GET / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.68.0\r\nAccept: */*\r\n\r\n");
//     std::cout << req.getMethod() << std::endl;
//     std::cout << req.getPath() << std::endl;
//     std::cout << req.getVersion() << std::endl;
//     std::cout<<"-----------------Headers-----------------"<<std::endl;
//     std::map<std::string, std::string> headers = req.getHeaders();
//     std::map<std::string, std::string>::iterator it;
//     for( it = headers.begin(); it != headers.end(); it++)
//     {
//         std::cout << it->first << "||" << it->second << std::endl;
//     }
//     return 0;
// }

