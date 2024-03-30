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
        std::string key = req.substr(i, k-i);
        int l = req.find("\r\n", k);
        std::string value = req.substr(k+2, l-k-2);
        this->headers[key] = value;
        i = l+2;
    }
    i += 2;
    this->body = req.substr(i, req.length()-i);
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
        post_handler(this->body);
    }
    else
    {
        std::cout<<"Invalid Method"<<std::endl;
    }
}

std::string Request::content_type_handler()
{
    std::map<std::string, std::string>::iterator it;
    for(it = this->headers.begin(); it != this->headers.end(); it++)
    {
        if(it->first == "Content-Type")
        {
            if(it->second == "application/json")
                return(".json");
            else if(it->second == "text/css")
                return(".css");
            else if(it->second == "image/gif")
                return(".gif");
            else if(it->second == "text/csv")
                return(".csv");
            else if(it->second == "text/html")
                return(".html");
            else if(it->second == "image/jpeg")
                return(".jpeg");
            else if(it->second == "text/javascript")
                return(".js");
            else if(it->second == "image/png")
                return(".png");
            else if(it->second == "text/plain")
                return(".txt");
            else if(it->second == "image/svg+xml")
                return(".svg");
            else if(it->second == "application/pdf")
                return(".pdf");
            else if(it->second == "image/x-icon")
                return(".ico");
            else if(it->second == "video/mp4")
                return(".mp4");
            else if(it->second == "audio/mpeg")
                return(".mp3");
            else
                std::cout<<"Invalid Content-Type"<<std::endl;
        }
    }
    return (0);
}

void Request::post_handler(std::string body)
{
    std::ofstream o;

    std::string filename = "post" + content_type_handler();
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