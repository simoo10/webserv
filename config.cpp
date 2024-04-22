

#include "config.hpp"
// bool isspace(const std::string& str)
// {
//     for(size_t i = 0 ; i < str.length();i++)
//     {
//         if(!isspace(str.at(i)))
//             return (false);
//     }
//     return (true);
// }
void setconfig(std::map<std::string,std::vector<std::string> > &map ,const std::vector<std::string> &arr)
{
    std::vector<std::string>::const_iterator it;
    for(it = arr.begin() ; it != arr.end() ; it++)
    {
        map[*it];
    }
}

Keywords::Keywords()
{
    _GlobalConfig["autoindex"].push_back("on");
    _GlobalConfig["autoindex"].push_back("off");
    _GlobalConfig["cgi"] = _GlobalConfig["autoindex"];
    _GlobalConfig["allowed_methods"].push_back("GET");
    _GlobalConfig["allowed_methods"].push_back("POST");
    _GlobalConfig["allowed_methods"].push_back("DELETE");

    _locationConfig = _serverConfig = _GlobalConfig;

    _GlobalConfig["error_log"];
    _GlobalConfig["log_level"];
    _GlobalConfig["log_file"];
    _GlobalConfig["access_log"];
    _GlobalConfig["default_type"];
    _GlobalConfig["include"];
    _GlobalConfig["redirect"];
    _GlobalConfig["error_page"];
    _GlobalConfig["index"];

    _serverConfig["listen"];
    _serverConfig["server_name"];
    _serverConfig["root"];
    _serverConfig["index"];
    _serverConfig["error_page"];
    _serverConfig["redirect"];
    _serverConfig["proxy_pass"];
    _serverConfig["access_log"];
    _serverConfig["error_log"];

    _locationConfig["root"];
    _locationConfig["alias"];
    _locationConfig["index"];
    _locationConfig["error_page"];
     _locationConfig["redirect"];
    _locationConfig["access_log"];
    _locationConfig["error_log"];
    _locationConfig["proxy_pass"];
    //gkeywords.insert(_GlobalConfig.begin(),_GlobalConfig.end());

    // gkeywords["log_level"];
    // gkeywords["log_file"];
    // gkeywords["error_log"];
    // gkeywords["default_type"];
    // gkeywords["root"];
    // gkeywords["index"];


    //keyspath.insert("log_level");
    keyspath.insert("log_file");
    keyspath.insert("error_log");
    keyspath.insert("root");
    keyspath.insert("index");
    keyspath.insert("alias");
    keyspath.insert("default_page");
    keyspath.insert("include");
   // keyspath.insert("error_page");
    keyspath.insert("access_log");
    //keyspath.insert("server_name");

    //keywords = gkeywords;
    //key.insert("server");
    //keywords["server_name"];
    
    //keywords["listen"];
    //keywords.insert("listen");
    
    //keywords.insert("path");
    //keywords["root"];
    //keywords["root"];
    //keywords["alias"];
    
    //keywords.insert("alias");
    //keywords["default_page"];
    //key.insert("location");
    // keywords["proxy_pass"];
    // keywords["include"];
    // keywords["autoindex"].push_back("on");
    // keywords["autoindex"].push_back("off");
    // keywords["cgi"] = keywords["autoindex"];
    // keywords["error_page"];
    // keywords["access_log"];
    // keywords["allowed_methods"].push_back("GET");
    // keywords["allowed_methods"].push_back("POST");
    // keywords["allowed_methods"].push_back("DELETE");

    //default_type application/octet-strem;
    //include /etc/nginx/mime.types;
    //index
    //autoindex
    //redirect
}

bool allOf(const std::string& str ,int(*fun)(int c)=isspace)
{
    for(size_t i = 0 ; i < str.length();i++)
    {
        if(!fun(str.at(i)))
            return (false);
    }
    return (true);
}
bool iscomment(const std::string& str)
{
    std::string::const_iterator it ;
    for(it = str.begin();  it != str.end() && isspace(*it);it++)
        ;
    if(it != str.end() && *it == '#')
    {
        return (true);   
    }
    return (false);
}

// void GlobalConfig::addkeywords()
// {
//     gkeywords.insert("log_level");
//     gkeywords.insert("log_file");
//     gkeywords.insert("error_log");
//     //keywords = gkeywords;
//     //key.insert("server");
//     keywords.insert("server_name");
    
//     keywords.insert("port");
//     //keywords.insert("listen");
    
//     //keywords.insert("path");
//     keywords.insert("root");
    
//     //keywords.insert("alias");
//     keywords.insert("default_page");
//     //key.insert("location");
//     keywords.insert("proxy_pass");
//     keywords.insert("include");
//     keywords.insert("error_page");
//     keywords.insert("access_log");

//     //default_type application/octet-strem;
//     //include /etc/nginx/mime.types;
//     //index
//     //allowed_methods
//     //autoindex

// }
// std::string removeChar(const std::string &str,char c)
// {
   
    
// }

std::vector<std::string> trim(const std::string &str , char c)
{
    std::vector<std::string> arr;
    std::string value;
    size_t pos,start;

    pos = 0;
    do
    {
        start = pos;
        pos = str.find(c,start);
        if(start < pos)
        {
            if(pos == std::string::npos)
                pos = str.length();
            value = str.substr(start,pos - start);
            if(!allOf(value,isspace) && !value.empty())
               arr.push_back(value);
        }else if(pos != std::string::npos)
        {
            pos++;  
        }
    }while (pos < str.length());
    return (arr);
}

std::vector<std::string> trim(const std::vector<std::string> arr , char c)
{
    std::vector<std::string> vector,data;
    std::vector<std::string>::const_iterator it ;

    for(it = arr.begin() ; it != arr.end() ; it++)
    {
        data = trim(*it,c);
        vector.insert(vector.end(),data.begin(),data.end());
    }
    return (vector);
}
bool isValidPath(const std::string &path)
{
    std::string::const_iterator it ;
    std::string str = "\\ ._~:/?#[]@!$&'()*+,;=-%"; 
    for(it = path.begin() ; it != path.end() ; it++)
    {
        if (!isalnum(*it) && str.find(*it) == std::string::npos)
        {
            return (false);
        }
    }   
    return (true);
}
bool isDir(const std::string &path)
{
    struct stat info;
    if(stat(path.c_str(),&info) != 0)
    {
        return (false);
    }
    return (false);
}
bool isIpAdresse(const std::string &str)
{
    std::vector<std::string> arr;
    std::vector<std::string>::iterator it;
    std::stringstream ss;
    int block;
    if(std::count(str.begin() , str.end() , '.') != 3)
        return (false);
    arr = trim(str,'.');
    if(arr.size() != 4)
        return (false);
    for(it = arr.begin() ; it  != arr.end();it++)
    {
        if(!allOf(*it,isdigit))
            return false;
        ss << *it;
        ss >> block;
        if(ss.fail()|| block > 255 || block <0)
            return false;
        ss.clear();
    }
    return (true);
}
bool checkBracket(const std::string &line)
{
    size_t left, right;

    left = line.find('[',0);
    right = line.find(']',0);
    
    if(left > right ||(left > 0 && !allOf(line.substr(0,left))) 
    || (right != (line.length() - 1) && !allOf(line.substr(right+1,line.length()))))
        {
            return (false);
        }
    return (true);
}
bool check(const std::vector<std::string>& arr,std::map<std::string,std::vector<std::string> > &map)
{
    for(size_t i = 1 ; i < arr.size();i++)
        {
            if((std::find(map[arr[0]].begin(),map[arr[0]].end(),arr.at(i)) == map[arr[0]].end()) 
            || std::find(arr.begin() + 1,arr.end(),arr.at(i)) != (arr.begin() + i))
                return (false);
        }
    return (true);
}
bool checkkeywords(const std::vector<std::string>& arr,std::map<std::string,std::vector<std::string> >& map)
{
    Keywords key;
    std::vector<std::string>::const_iterator it = arr.begin();
    std::vector<std::string>::iterator it_map; 
    if(map.find(arr.at(0)) == map.end() || (*it == "listen" && !allOf(*(it + 1), isdigit) && atoi((*(it + 1)).c_str()) <= 65535)
    || (*it != "allowed_methods" && *it != "error_page" && *it != "index" && *it != "redirect" && arr.size() != 2) )
    {
        return (false);
    }else if(((*it == "autoindex" || *it == "cgi" )&& (arr.size() != 2 || !check(arr,map)))
    || (*it == "allowed_methods" && (arr.size() > 4 || !check(arr,map))))
    {
        return (false);
    }
    else if(((*it == "error_page" || *it == "redirect") && (!allOf(*(it + 1), isdigit) || arr.size() != 3  || !isValidPath(arr.at(2))))
    || (key.keyspath.find(arr[0]) != key.keyspath.end() && !isValidPath(arr.at(1))))
    {
        return (false);
    }
    return (true);
}
void DefaultParameter( std::map < std::string, serverConfig > &map)
{
    std::vector<std::string> arr;
    std::map < std::string, serverConfig >::iterator it;
    arr.push_back("cgi");
    arr.push_back("autoindex");
    arr.push_back("listen");
    
    for(it = map.begin();it!= map.end();it++)
    {
    for(size_t i = 0 ; i < arr.size() ; i++)
    {
        if(it->second.config.find(arr[i]) == it->second.config.end())
        {
            if(arr[i] != "listen")
                it->second.config[arr[i]].push_back("off");
            else
                it->second.config[arr[i]].push_back("80");
        }
    }
    }
}
GlobalConfig::GlobalConfig(const char *fileName): Keywords()
{
    size_t left,right;
    std::ifstream configFile(fileName);
    std::string line;
    std::vector<std::string> arr,data;
    
    if(!configFile.is_open())
        throw std::runtime_error("Error: Unable to open configuration file");
    while (std::getline(configFile,line))
    {
        if(line.empty() || allOf(line,isspace) || iscomment(line))
            continue;
        else if( (left = line.find('[')) != std::string::npos && (right = line.find(']')) != std::string::npos && checkBracket(line))
        {
            arr = trim(line.substr(left + 1,right-left-1),' ');
            if(arr.size() != 2)
                throw std::runtime_error(INVALIDCONFIG);
            if(arr[0] == "server" && isIpAdresse(arr[1]) && servers.find(arr.at(1)) == servers.end())
            {
                currentServer = arr.at(1);
                currentLocation.clear();
            }
            else if(arr[0] == "location" && isValidPath(arr[1]) && !servers.empty() 
            && servers[currentServer].locations.find(arr[1]) == servers[currentServer].locations.end())
            {
                currentLocation = arr[1];
            }else
                throw std::runtime_error(INVALIDCONFIG);
        } else if(std::count(line.begin(),line.end(),'=') == 1)
        {
            arr = trim(trim(line , '='),' ');
            if(arr.empty() || arr.size() < 2 )
                throw std::runtime_error(INVALIDCONFIG);
            setServers(arr);
        }else
            throw std::runtime_error(INVALIDCONFIG);
    }
    configFile.close();
    if(config.empty() && servers.empty())
        throw std::runtime_error(INVALIDCONFIG);
    DefaultParameter(servers);
}

// if((currentServer.empty() && gkeywords.find(*it) == gkeywords.end()) 
//     || (!currentServer.empty() && keywords.find(*it) == keywords.end()))
//     {
//         return (false);
//     }
void GlobalConfig::setServers(const std::vector<std::string>& arr)
{
    // if(_GlobalConfig.find(arr[0]) == _GlobalConfig.end() && _serverConfig.find(arr[0]) == _serverConfig.end())
    // {
    //     throw std::runtime_error("SERVER");
    // }else

    if(currentServer.empty())
    {
        if(_GlobalConfig.find(arr[0]) == _GlobalConfig.end() || !checkkeywords(arr,_GlobalConfig))
                   throw std::runtime_error(INVALIDCONFIG);
        setconfig(arr);
    }else
    {
        if(currentLocation.empty())
            {
                if(_serverConfig.find(arr[0]) == _serverConfig.end() || !checkkeywords(arr,_serverConfig))
                   throw std::runtime_error(INVALIDCONFIG);
                servers[currentServer].setconfig(arr);
            }
        else{
             if(_locationConfig.find(arr[0]) == _locationConfig.end() || !checkkeywords(arr,_locationConfig))
                throw std::runtime_error(INVALIDCONFIG);
            servers[currentServer].locations[currentLocation].setlocation(arr);
            }
    }
}

const std::map<std::string,std::vector<std::string> >& GlobalConfig::getGlobalConfig() const
{
    return (config);
}
void GlobalConfig::setconfig(const std::vector<std::string> &arr)
{
    std::map < std::string, std::vector<std::string> >::iterator it;
    std::vector<std::string>::const_iterator first = arr.begin() + 1;
    std::vector<std::string>::const_iterator last = arr.end();
   if( (it = config.find(arr[0])) != config.end())
    {
         if(std::find(it->second.begin(),it->second.end(),arr[1]) == it->second.end())
           it->second.insert(it->second.end(),first,last);
        else
            throw std::runtime_error(INVALIDCONFIG);
    }else
    {
        config[arr[0]].assign(first,last);
    }
}
void locationConfig::setlocation(const std::vector<std::string> &arr)
{
    std::map < std::string, std::vector<std::string> >::iterator it;
    std::vector<std::string>::const_iterator first = arr.begin()+1;
    std::vector<std::string>::const_iterator last = arr.end();
    
    if( (it = config.find(arr[0])) != config.end())
    {
        if(std::find(it->second.begin(),it->second.end(),arr[1]) == it->second.end())
           it->second.insert(it->second.end(),first,last);
        else
            throw std::runtime_error(INVALIDCONFIG);
    }else
    {
        config[arr[0]].assign(first,last);
    }
}

void serverConfig::setconfig(const std::vector<std::string> &arr)
{
    std::map < std::string, std::vector<std::string> >::iterator it;
    std::vector<std::string>::const_iterator first = arr.begin()+1;
    std::vector<std::string>::const_iterator last = arr.end();

    if( (it = config.find(arr[0])) != config.end())
    {
        if(std::find(it->second.begin(),it->second.end(),arr[1]) == it->second.end())
            it->second.insert(it->second.end(),first,last);
        else
            throw std::runtime_error(INVALIDCONFIG);
    }else
        config[arr[0]].assign(first,last);
}

std::string GlobalConfig::getLocation(const std::string URI, const std::string& currentServer) const {
    // get the matching location block for the URI
    std::map<std::string, locationConfig> locations;

    cerr << "URI : " << URI << " currentServer : " << currentServer << std::endl;
    if (servers.count(currentServer) > 0) {
        locations = servers.at(currentServer).locations;
    }
    for (std::map<std::string, locationConfig>::iterator it = locations.end(); it != locations.begin(); it--) {
        cout << "Location : " << it->first << std::endl;
        if (URI.find(it->first) == 0) {
            return it->first;
        }
    }
    return "";
}

std::vector<std::string> GlobalConfig::getDefaultPages( const std::string& currentLocation,
                        const std::string& currentServer) const {
    std::map<std::string, locationConfig> locations;
    std::map<std::string, serverConfig> servers;
    std::map<std::string, std::vector<std::string> > config;
    std::string location;
    std::string server;

    if (servers.count(currentServer) > 0) {
        locations = servers.at(currentServer).locations;
    }
    if (servers.count(currentServer) > 0) {
        config = servers.at(currentServer).config;
    }

    // Check location block first (highest priority)
    if (!currentLocation.empty()) {
        if (locations.count(currentLocation) > 0) {
            if (locations.at(currentLocation).config.count("default_page") > 0) {
                return locations.at(currentLocation).config.at("default_page");
            }
        }
    }

    // Check server block if location not set or doesn't have default_page
    if (!currentServer.empty()) {
        if (servers.count(currentServer) > 0) {
            if (servers.at(currentServer).config.count("default_page") > 0) {
                return servers.at(currentServer).config.at("default_page");
            }
        }
    }

    config = this->config;
    // Check global config block if not set in server or location
    if (config.count("default_page") > 0) {
        return config.at("default_page");
    }

    // Default to empty string if not set in any block
    std::vector<std::string> arr;
    return arr;

}

std::string GlobalConfig::getRoot( const std::string& currentLocation,
                        const std::string& currentServer) const {
    std::map<std::string, locationConfig> locations;
    std::map<std::string, serverConfig> servers;
    std::map<std::string, std::vector<std::string> > config;
    std::string location;
    std::string server;

    if (servers.count(currentServer) > 0) {
        locations = servers.at(currentServer).locations;
    }
    if (servers.count(currentServer) > 0) {
        config = servers.at(currentServer).config;
    }

    // Check location block first (highest priority)
    if (!currentLocation.empty()) {
        if (locations.count(currentLocation) > 0) {
            if (locations.at(currentLocation).config.count("root") > 0) {
                return locations.at(currentLocation).config.at("root").at(0);
            }
        }
    }

    // Check server block if location not set or doesn't have root
    if (!currentServer.empty()) {
        if (servers.count(currentServer) > 0) {
            if (servers.at(currentServer).config.count("root") > 0) {
                return servers.at(currentServer).config.at("root").at(0);
            }
        }
    }

    config = this->config;
    // Check global config block if not set in server or location
    if (config.count("root") > 0) {
        return config.at("root").at(0);
    }

    // Default to empty string if not set in any block
    return "";
}

bool GlobalConfig::isCGIAllowed( const std::string& currentLocation,
                            const std::string& currentServer) const {
    std::vector <std::string> arr;
    arr.push_back("on");
    // Check location block first (highest priority)
    if (!currentLocation.empty()) {
        if (servers.count(currentServer) > 0) {
            std::map<std::string, locationConfig> locations = this->servers.at(currentServer).locations;
            if (locations.count(currentLocation) > 0) {
                if (locations.at(currentLocation).config.count("cgi") > 0) {
                    return (locations.at(currentLocation).config.at("cgi") == arr);
                }
            }
        }
    }

    // Check server block if location not set or doesn't have cgi
    if (!currentServer.empty()) {
        if (servers.count(currentServer) > 0) {
            if (servers.at(currentServer).config.count("cgi") > 0) {
                return (servers.at(currentServer).config.at("cgi") == arr);
            }
        }
    }

    // Check global config block if not set in server or location
    if (config.count("cgi") > 0) {
        return (config.at("cgi") == arr);
    }

    // Default to off if not set in any block
    return false;
}


bool GlobalConfig::isAutoindexAllowed( const std::string& currentLocation,
                            const std::string& currentServer) const {
    std::vector <std::string> arr;
    arr.push_back("on");
    // Check location block first (highest priority)
    if (!currentLocation.empty()) {
        if (servers.count(currentServer) > 0) {
            std::map<std::string, locationConfig> locations = this->servers.at(currentServer).locations;
            if (locations.count(currentLocation) > 0) {
                if (locations.at(currentLocation).config.count("autoindex") > 0) {
                    return (locations.at(currentLocation).config.at("autoindex") == arr);
                }
            }
        }
    }

    // Check server block if location not set or doesn't have autoindex
    if (!currentServer.empty()) {
        if (servers.count(currentServer) > 0) {
            if (servers.at(currentServer).config.count("autoindex") > 0) {
                return (servers.at(currentServer).config.at("autoindex") == arr);
            }
        }
    }

    // Check global config block if not set in server or location
    if (config.count("autoindex") > 0) {
        return (config.at("autoindex") == arr);
    }

    // Default to off if not set in any block
    return false;
}

// void afficher(const std::vector<std::string> &arr)
// {
//     for(size_t i = 0;i < arr.size();i++)
//     {
//         std::cout << " " <<arr.at(i);
//     }
//     std::cout << std::endl;
// }

// void GlobalConfig::getconfig()
// {
//     std::map <std::string,  std::vector<std::string> >::iterator it;
//     std::map < std::string, serverConfig > ::iterator it_server;
//     std::map < std::string, locationConfig > ::iterator it_location;
//     std::map <std::string,  std::vector<std::string> >::iterator it_config;
//     if(!config.empty())
//     {
//         std::cout << "# Global Configuration" << std::endl;
//     }
//     for(it_config = config.begin(); it_config != config.end();it_config++)
//     {
//         std::cout << it_config->first << " ";
//          afficher(it_config->second);
//     }
//     if(!servers.empty())
//     {
//         std::cout << "# Server Configurations" << std::endl;
        
//         for(it_server = servers.begin() ; it_server != servers.end() ; it_server++)
//         {
//             std::cout << "server :" << it_server->first << std::endl;
//             it_server->second.getconfig();
//             if(!it_server->second.locations.empty())
//             {
//                 for(it_location = it_server->second.locations.begin() ; it_location != it_server->second.locations.end() ; it_location++)
//                 {
//                     std::cout << "Location : " << it_location->first << std::endl;
//                     it_location->second.getconfig();
//                 }
//             }
//         }
//     }
// }

// void serverConfig::getconfig()
// {
//     std::map <std::string,  std::vector<std::string> >::iterator it;
    
//     for(it = config.begin();it != config.end();it++)
//     {
//         std::cout << it->first << " = ";
//         afficher(it->second);
//     }
// }

// void locationConfig::getconfig()
// {
//     std::map <std::string,  std::vector<std::string> >::iterator it;
//     for(it = config.begin();it != config.end();it++)
//     {
//         std::cout << it->first << " =";
//         afficher(it->second);
//     }
// }
