#include"request.hpp"
#include<unistd.h>
Request::Request(){
    this->method = "";
    this->path = "";
    this->version = "";
    this->headers = std::map<std::string, std::string>();
    this->body = "";
    this->status = 200;
    this->header_status = false;
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

int Request::parseheaders(std::string req)
{
    int i = 0;
    int pip = 0;
    pip = req.find("\r\n"); 
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
        if(req[i] == '\r' && req[i+1] == '\n')
            header_status = true;
    }
    i += 2;
    return(i);
}

int Request::parseRequest(char *req,int bytesRead)
{
    int i = 0;
    std::string reqstr;
    reqstr.append(req, bytesRead);
    if(reqstr.find("\r\n\r\n") == std::string::npos)
    {
        return (1);
    }
    while(header_status == false)
        i = parseheaders(reqstr);
   // i = reqstr.length() - i;
    std::cout<<"i=====>"<<i<<std::endl;
    std::cout<<"--->>>"<<headers["content-length"]<<std::endl;
    std::cout<<"==/>"<<bytesRead<<std::endl;
    // if(i < reqstr.length())
    // {
        // if(headers["Transfer-Encoding"]=="chunked")
        // {    
        //     chunked_request_handler(req);
        // }
        // else
        //{
            //this->body = req.substr(i, req.length()-i);
            method_handler(this->method,req+i);
       // }
    //}
    return(0);
}

void Request::method_handler(std::string method,char *body)
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
        post_handler(body);
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

void Request::post_handler(char *body)
{
    std::ofstream o;
    if(strlen(body) == 0)
        return;
    // std::string value = headers["Content-Length"];
    // if(body.length()==std::atoi(value.c_str()))
    // {
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

// void Request::chunked_request_handler(std::string bd)
// {
//     std::string chunk;
//     int i = 0;
//     int chunksize=0 ;
//     while(i<bd.length())
//     {
//         int pip = bd.find("\r\n", i);
//         if (pip == std::string::npos) {
//             std::cerr << "Error" << std::endl;
//             return;
//         }
//         chunksize = hexatoint(bd.substr(i, pip-i));
//         if(chunksize == 0)
//             break;
//         i = pip+2;
//         if (i + chunksize > bd.length()) {
//             std::cerr << "Error: Incomplete data" << std::endl;
//             return;
//         }
//         chunk.append(bd.substr(i, chunksize));
//         i += chunksize+2;
//     }
//     post_handler(chunk);
// }

void Request::request_status_code()
{ 
    if(method.empty() || path.empty() || version.empty())
        status = 400;
    else if(path.length() > 2048)
        status = 414;
    else if(version != "HTTP/1.1")
        status = 505;
    else if(method != "GET" && method != "POST" && method != "DELETE")
    {
        if(method == "PUT" || method == "HEAD" || method == "OPTIONS" || method == "TRACE" || method == "CONNECT")
            status = 501;
        else
            status = 400;
    }
    else if(method == "POST" )
    {
        if(headers.find("Content-Length") == headers.end() && headers.find("Transfer-Encoding") == headers.end())
            status = 411;
        else if(headers.find("Content-Length") != headers.end() && headers.find("Transfer-Encoding") != headers.end())
            status = 400;
        else if(headers.find("Content-Length") != headers.end() && headers.find("Transfer-Encoding") == headers.end())
        {
            if(headers["Content-Length"] == "0")
                status = 400;
        }
        else if(headers.find("Transfer-Encoding") != headers.end() && headers.find("Content-Length") == headers.end())
        {
            if(headers["Transfer-Encoding"] != "chunked")
                status = 400;
        }
        else if(headers.find("Content-Type") == headers.end())
            status = 400;
        
    }

}