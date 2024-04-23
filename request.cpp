#include"request.hpp"
#include<unistd.h>
#include"config.hpp"
Request::Request(){
    this->method = "";
    this->path = "";
    this->version = "";
    this->headers = std::map<std::string, std::string>();
    this->body = "";
    this->status = 201;
    this->header_status = false;
    this->bodylength = 0;
    this->content_length = 0;
    this->bytes_read = 0;
    this->file_status = false;
    this->chunked_status = false;
    chunksize = 0;
    flag = false;
    iposition = 0;
    hexa_status = false;
}

Request::Request(std::string method, std::string path, std::string version, std::map<std::string, std::string> headers, std::string body){
    this->method = method;
    this->path = path;
    this->version = version;
    this->headers = headers;
    this->body = body;
    this->method = "";
    this->path = "";
    this->version = "";
    this->headers = std::map<std::string, std::string>();
    this->body = "";
    this->status = 200;
    this->header_status = false;
    this->bodylength = 0;
    this->content_length = 0;
    this->bytes_read = 0;
    this->file_status = false;
    this->chunked_status = false;
    chunksize = 0;
    flag = false;
    iposition = 0;
    hexa_status = false;
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

int Request::parseheaders(std::string req,GlobalConfig &config)
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

int Request::parseRequest(char *req,int bytesRead,GlobalConfig &config)
{
    int i = 0;
    std::string reqstr;
    bytes_read = bytesRead;
    if(header_status == false)
    {
        reqstr.append(req, bytesRead);
        if(reqstr.find("\r\n\r\n") == std::string::npos)
        {
            return (1);
        }
        //std::cout<<"----------------------"<<std::endl;
        i = parseheaders(reqstr,config);
    std::string value = headers["Content-Length"];
    content_length = std::atoi(value.c_str());
    }
    if(headers["Content-Length"] == "0")
    {
        status = 400;
        std::cout<<"----------------------"<<std::endl;
        std::string response = std::to_string(status) + " " + "Bad Request" + "\r\n";
        send(clientSocket, response.c_str(), response.length(), 0);
        return(0);
    }
    if(headers["Transfer-Encoding"]=="chunked")
    {
        // cout << i << endl;
        chunked_request_handler(req,i);
    }
    else
        method_handler(this->method,req+i, i);
    i = 0;
    //std::cout<<"----------------------"<<std::endl;
    return(0);
}

void Request::method_handler(std::string method,char *body, int i)
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
        post_handler(body, i);
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
void Request::post_handler(char *body, int i)
{
    std::ofstream o;
    long l = std::min (content_length, (long)bytes_read - i);
    std::string filename = "post" + content_type_handler();
	o.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);
    o.write(body, l);
    o.close();
    content_length -= l;
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

void Request::chunked_request_handler(char *body,int i)
{
    //int  i = 0;
    int j = 0;
    //long chunksize = 0;
    std::ofstream o;
    long readed = 0;
    long l = 0;
    std::string filename = "post" + content_type_handler();
    static std::string hex;
    static int hexa_end;
    
    while(i < bytes_read)
    {
        if(flag == false)
        {
       if (!check)
       {
                //cout << i << " " << bytes_read << endl;
               // cout << (int)body[i] << endl;
                //cout << "1" << endl;
                if (body[i] == '\r')
                {
                   // cout << "here 1" << endl;
                    i ++;
                    continue;
                }
                if (body[i] == '\n')
                {
                    //cout << "here 2" << endl;
                    i ++;
                    hexa_end++;
                    continue;
                }
        //cout << "2" << endl;

                j = i;
                if(hexa_end<2)
                {
                while(body[j] != '\r' && j < bytes_read)
                {
                    // std::cout << body[j] << "-----" << j << std::endl;
                    //std::cout<<body[j]<<"-----"<<j<<std::endl;
                    //sleep(1);
                    j++;
                }
               // std::cout<<(int)body[j]<<"-----=="<<j<<std::endl;
                //std::cout<<body[j+1]<<"-----=="<<j+1<<std::endl;
                //std::cout<<body[j+2]<<"-----=="<<j+2<<std::endl;
                for(int k = i; k < j; k++) {
                    hex += body[k];
                   // cout << body[k] << " ";
                }
                //cout << endl;
                }
                
                if (j == bytes_read)
                {
                    break;
                }
               // cout << "3" << endl;
                chunksize = hexatoint(hex);
               // cout << "chunksize: " << chunksize << endl;
                hex.clear();
                if(chunksize == 0)
                    { header_status=false ; flag = true; return; }
                check = true;
                i = j;
                if(body[i] == '\r')
                    i++;
                if(body[i] == '\n')
                    i++;
                //std::cout<<i<<"==>>"<<body[i]<<std::endl;
            }
            else {
                if (body[i] == '\r')
                {
                    i ++;
                    continue;
                }
                if (body[i] == '\n')
                {
                    i ++;
                    continue;
                }
                flag = true;
                check = false;
                continue;
            }
        }
        hexa_end = 0;
        //cout<< "chunksize: " << chunksize << " i: " << i << " bytes_read: " << bytes_read << endl;
        l = std::min (chunksize, (long)(bytes_read - i));
        o.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);
        if(!o.is_open())
           status = 500;
        o.write(body + i, l);
        if(!o)
            status = 500;
        o.flush();
        o.close();
        chunksize -= l;
        i += l ;
        if(chunksize == 0)
        {
            j = 0;
            check = false;
            flag = false;
        }
    }
}

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
        else if(headers["content-type"] == "boundary")
            status = 400;
    }

}