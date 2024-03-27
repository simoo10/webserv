#include"request.hpp"

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

void Request::parseRequest(std::string req)
{
    int i = 0;
    int pip = 0;
    pip = req.find("\r\n");
   // std::cout<<"===>"<<pip<<std::endl;
    std::string reqline = req.substr(0, pip);
    while(reqline[i])
    {
        if(reqline[i] == ' ')
        {
            this->method = reqline.substr(0, i);
            break;
        }
        i++;
    }
    int j = i+1;
    while(reqline[j])
    {
        if(reqline[j] == ' ')
        {
            this->path = reqline.substr(i+1, j-i-1);
            break;
        }
        j++;
    }
    this->version = reqline.substr(j+1, reqline.length()-j-1);
    i = pip+2;
    while(req[i] != '\r' && req[i+1] != '\n')
    {
        int k = req.find(":", i);
        //std::cout<<"//===>"<<k<<std::endl;
        std::string key = req.substr(i, k-i);
        int l = req.find("\r\n", k);
        // std::cout<<"////===>"<<l<<std::endl;
        std::string value = req.substr(k+2, l-k-2);
        this->headers[key] = value;
        // std::cout<<"***////===>"<<key<<std::endl;
        // std::cout<<"**////===>"<<value<<std::endl;
        i = l+2;
    }
    i += 2;
    std::cout<<"===>"<<i<<std::endl;
    std::cout<<"===>"<<req.length()-i<<std::endl;
    std::cout<<"===>"<<req.length()<<std::endl;
    this->body = req.substr(i, req.length()-i);
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
        post_handler(this->body);
    }
    else
    {
        std::cout<<"Invalid Method"<<std::endl;
    }
}

void Request::post_handler(std::string body)
{
    std::ofstream o;
    std::string filename = "post.txt";
	o.open(filename.c_str());
    o << body;
    o.close();
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