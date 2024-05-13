#include"request.hpp"
#include<unistd.h>
#include"config.hpp"
Request::Request(){
    this->method = "";
    this->path = "/home/kali/Desktop/webserv/www";
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
    request_status = false;
    check = false;
    clientSocket = 0;
    root_path = "/home/kali/Desktop/webserv/www"; //need to update after generating request 
    querystr = "";
    set_possible_headers();
    generate_filenames();
    body_size = 0;
    headerkey_status = false;
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
    request_status = false;
    check = false;
    clientSocket = 0;
    root_path = "/home/kali/Desktop/webserv/www"; //need to update after generating request
    querystr = "";
    set_possible_headers();
    body_size = 0;
    headerkey_status = false;
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
            if(i == 0)
            {
                status = 400;
                return(0);
            }
            this->method = reqline.substr(0, i);
            break;
        }
        i++;
    }
    if(method == "GET" || method == "POST")
    {
        if(method == "POST")
            status = 201;
        else
            status = 200;
    }
    int j = i+1;
    if(reqline[j] == ' ')
    {
        status = 400;
        return(0);
    }
    while(reqline[j])
    {
        if(reqline[j] == ' ')
        {
            this->path = reqline.substr(i+1, j-i-1);
            break;
        }
        j++;
    }
    if(reqline[j+1] == ' ')
    {
        status = 400;
        return(0);
    }
    this->version = reqline.substr(j+1, reqline.length()-j-1);
    i = pip+2;
    while(req[i] != '\r' && req[i+1] != '\n')
    {
        int k = req.find(":", i);
        if(k == std::string::npos)
        {
            status = 400;
            return(0);
        }
        std::string key = req.substr(i, k-i);
        int l = req.find("\r\n", k);
        std::string value = req.substr(k+2, l-k-2);
        this->headers[key] = value;
        i = l+2;
        if(req[i] == '\r' && req[i+1] == '\n')
        {
            std::cout<<"here100"<<std::endl;
            header_status = true;
        }
    }
    if(header_status == false)
    {
        status = 400;
        return(0);
    }
    set_querystr();
    check_path_availability();
    i += 2;
    return(i);
}
void Request::set_querystr()
{
    int pos = path.find("?");
    if(pos != std::string::npos)
    {
        querystr = path.substr(pos+1);
        path = path.substr(0, pos);
    }
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
        // std::cout<<"----------------------"<<std::endl;
        i = parseheaders(reqstr,config);
    std::string value = headers["Content-Length"];
    content_length = std::atoi(value.c_str());
    bodylength = std::atoi(value.c_str());
    }
   // std::cout<<"----------------------"<<std::endl;
   // std::cout<<req<<std::endl;
   // std::cout<<"----------------------"<<std::endl;
    // std::cout<<"------------------"<<method<<"---------------"<<std::endl;
    // if(method == "GET")
    // {
    //     CGI cgi;
    //     cgi.execute_cgi(*this);
    // }
    if(headerkey_status == false)
        check_headers();
    request_status_code();
    // if(status != 200 && status != 201)
    // {
        std::cout<<status<<std::endl;
        //std::cout<<"here1"<<std::endl;
        //send error response;
    //     return(0);
    // }
    if(headers["Content-Length"] == "0" && method == "POST")
    {
        status = 400;
        std::cout<<"----------------------"<<std::endl;
        // std::string response = std::to_string(status) + " " + "Bad Request" + "\r\n";
        // send(clientSocket, response.c_str(), response.length(), 0);
        return(0);
    }
    //handle body;
    if(headers["Transfer-Encoding"]=="chunked")
    {
        // cout << i << endl;
        chunked_request_handler(req,i);
    }
    else
    {
        std::cout<<"here2"<<std::endl;
        method_handler(this->method,req+i, i);
    }
    i = 0;
    // std::cout<<"----------------------"<<std::endl;
    return(0);
}

void Request::method_handler(std::string method,char *body, int i)
{
    if(method == "GET")
    {
        getMeth(*this);
    }
    else if(method == "DELETE")
    {
        deleteMeth(*this);
    }
    else if(method == "POST")
    {
        post_handler(body, i);
    }
    //else
    // {
    //     std::cout<<"Invalid Method"<<std::endl;
    // }
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
    if(it == this->headers.end())
    {
        status = 400;
    }
    return (0);
}

void Request::post_handler(char *body, int i)
{
    std::ofstream o;
    //generate_filenames();
    long l = std::min (content_length, (long)bytes_read - i);
    std::string filename = randfilename + content_type_handler();
	o.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);
    o.write(body, l);
    o.close();
    content_length -= l;
    if(content_length == 0)
    {
        std::ifstream file;
        file.open(filename.c_str(), std::ios::binary);
        if(!file.is_open())
            status = 500;
        file.seekg(0, std::ios::end);
        body_size = file.tellg();
        file.seekg(0, std::ios::beg);
        file.close();
        if(body_size != bodylength)
            status = 400;
        else
            status = 201;
        request_status = true;
        header_status=false ;
        return;
    }
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
    //generate_filenames();
    std::string filename = randfilename + content_type_handler();
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
                    {
                        std::ifstream file;
                        file.open(filename.c_str(), std::ios::binary);
                        if(!file.is_open())
                            status = 500;
                        file.seekg(0, std::ios::end);
                        body_size = file.tellg();
                        file.close();
                        if(body_size != bodylength)
                            status = 400;
                        else
                            status = 201;
                        request_status = true;
                        header_status=false ; 
                        flag = true; 
                        return; }
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
    {
        std::cout<<"here10"<<std::endl;
        status = 400;
    }
    else if(path.length() > 2048)
    {
        std::cout<<"here11"<<std::endl;
        status = 414;
    }
    //if the body size defined in the config file is less than the actual body size then 413
    else if(version != "HTTP/1.1")
    {
        std::cout<<"here12"<<std::endl;
        status = 505;
    }
    else if(method != "GET" && method != "POST" && method != "DELETE")
    {
        if(method == "PUT" || method == "HEAD" || method == "OPTIONS" || method == "TRACE" || method == "CONNECT")
            status = 501;
        else
            status = 405;//method not allowed;
    }
    else if(method == "POST" )
    {
        if(headers.find("Content-Length") == headers.end() && headers.find("Transfer-Encoding") == headers.end())
        {
            std::cout<<"here4"<<std::endl;
            status = 411;
        }
        else if(headers.find("Transfer-Encoding") != headers.end() && headers.find("Content-Length") == headers.end())
        {
            if(headers["Transfer-Encoding"] != "chunked")
            {
                std::cout<<"here7"<<std::endl;
                status = 400;
            }
        }
        else if(headers.find("Content-Type") == headers.end())
        {
            std::cout<<"here8"<<std::endl;
            status = 400;
        }
        else if(headers["content-type"] == "boundary")
        {
            std::cout<<"here9"<<std::endl;
            status = 400;
        }
    }

}

void Request::set_possible_headers()
{
    possible_headers.push_back("A-IM");
    possible_headers.push_back("Accept");
    possible_headers.push_back("Accept-Charset");
    possible_headers.push_back("Accept-Encoding");
    possible_headers.push_back("Accept-Language");
    possible_headers.push_back("Accept-Datetime");
    possible_headers.push_back("Access-Control-Request-Method");
    possible_headers.push_back("Access-Control-Request-Headers");
    possible_headers.push_back("Authorization");
    possible_headers.push_back("Cache-Control");
    possible_headers.push_back("Connection");
    possible_headers.push_back("Content-Length");
    possible_headers.push_back("Content-MD5");
    possible_headers.push_back("Content-Type");
    possible_headers.push_back("Cookie");
    possible_headers.push_back("Date");
    possible_headers.push_back("Expect");
    possible_headers.push_back("Forwarded");
    possible_headers.push_back("From");
    possible_headers.push_back("Host");
    possible_headers.push_back("If-Match");
    possible_headers.push_back("If-Modified-Since");
    possible_headers.push_back("If-None-Match");
    possible_headers.push_back("If-Range");
    possible_headers.push_back("If-Unmodified-Since");
    possible_headers.push_back("Max-Forwards");
    possible_headers.push_back("Origin");
    possible_headers.push_back("Pragma");
    possible_headers.push_back("Postman-Token");
    possible_headers.push_back("Proxy-Authorization");
    possible_headers.push_back("Range");
    possible_headers.push_back("Referer");
    possible_headers.push_back("TE");
    possible_headers.push_back("Transfer-Encoding");
    possible_headers.push_back("User-Agent");
    possible_headers.push_back("Upgrade");
    possible_headers.push_back("Via");
    possible_headers.push_back("Warning");
}

void Request::check_headers()
{
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator it1;
    // for(it1 = headers.begin(); it1 != headers.end(); it1++)
    // {
    //     std::cout<<it1->first<<"--->"<<it1->second<<std::endl;
    // }
    //exit(0);
    for(it = headers.begin(); it != headers.end(); it++)
    {
        std::cout<<it->first<<std::endl;
        if(std::find(possible_headers.begin(), possible_headers.end(), it->first) == possible_headers.end())
        {
            std::cout<<it->first<<std::endl;
            std::cout<<it->second<<std::endl;
            std::cout<<"here3"<<std::endl;
            // exit(0);
            // status = 400;
            // return;
        }
    }
    headerkey_status = true;
}

void Request::check_path_availability()
{
    if(path == "/")
        path = "/index.html";
    if(access((root_path + path).c_str(), F_OK) == -1)
        status = 404;
    path = root_path + path;
    std::string possible_caracters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=";
    for(int i = 0; i < path.length(); i++)
    {
        if(possible_caracters.find(path[i]) == std::string::npos)
        {
            status = 400;
            return;
        }
    }
}

void Request::generate_filenames()
{
    const std::string caracters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
    std::srand(std::time(0));
    for (int i = 0; i < 6; i++) {
        randfilename += caracters[std::rand() % caracters.length()];
    }

}

bool check_space(std::string str)
{
    for(int i = 0; i < str.length(); i++)
    {
        if(str[i] == ' ')
        {
            if(str[i+1] == ' ')
                return (false);
        }
    }
    return (true);
}

void Request::check_headers_content()
{
    std::map<std::string, std::string>::iterator it;
    for(it = headers.begin(); it != headers.end(); it++)
    {
        if(it->second.empty() || it->first.empty() || !check_space(it->first) || !check_space(it->second))
        {
            status = 400;
            return;
        }
        if(it->first == "Content-Length")
        {
            if(it->second.find_first_not_of("0123456789") != std::string::npos)
            {
                status = 400;
                return;
            }
        }
    }
}

void Request::required_headers()
{
    std::vector<std::string> required_headers;
    if(method == "POST")
    {
        required_headers.push_back("Content-Length");
        required_headers.push_back("Content-Type");
        required_headers.push_back("Host");
        std::map<std::string, std::string>::iterator it;
        for(int i = 0; i < required_headers.size(); i++)
        {
            it = headers.find(required_headers[i]);
            if(it == headers.end())
            {
                status = 400;
                return;
            }
        }
    }
    else if(method == "GET")
    {
        required_headers.push_back("Host");
        std::map<std::string, std::string>::iterator it;
        for(int i = 0; i < required_headers.size(); i++)
        {
            it = headers.find(required_headers[i]);
            if(it == headers.end())
            {
                status = 400;
                return;
            }
        }
    }
    else if(method == "DELETE")
    {
        required_headers.push_back("Host");
        std::map<std::string, std::string>::iterator it;
        for(int i = 0; i < required_headers.size(); i++)
        {
            it = headers.find(required_headers[i]);
            if(it == headers.end())
            {
                status = 400;
                return;
            }
        }
    }
}