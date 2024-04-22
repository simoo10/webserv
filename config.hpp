#ifndef CONFIG_HPP
#define CONFIG_HPP
#define INVALIDCONFIG "Error: invalid configuration file"

#include "message.hpp"
#include"request.hpp"
//#define npos std::string::npos
#include <set>
#include <map>
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define BOLDBLACKB   "\033[1;40m"           /* Bold Black BackGround*/
#define BOLDREDB     "\033[1;41m"      /* Bold Red BackGround*/
#define BOLDGREENB   "\033[1;42m"      /* Bold Green BackGround*/
#define BOLDYELLOWB  "\033[1;43m"      /* Bold Yellow BackGround*/
#define BOLDBLUEB    "\033[1;44m"      /* Bold Blue BackGround*/
#define BOLDMAGENTAB "\033[1;45m"      /* Bold Magenta BackGround*/
#define BOLDCYANB    "\033[1;46m"      /* Bold Cyan BackGround*/
#define BOLDWHITEB   "\033[1;47m"     /* Bold White BackGround*/

class Keywords
{
    protected:
    std::map<std::string , std::vector<std::string> > _GlobalConfig ;
    std::map<std::string , std::vector<std::string> > _serverConfig ;
    std::map<std::string , std::vector<std::string> > _locationConfig ;
    public:
    std::set<std::string> keyspath;
    void setkeywords();
    Keywords();
};

class locationConfig {
    public :
        std::map < std::string, std::vector<std::string> > config;
        void setlocation(const std::vector<std::string> &);
        //void getconfig();
        //std::map<std::string ,  std::vector<std::string> >& getlocations();
};

class serverConfig {
    public :
        std::map < std::string, std::vector<std::string> > config;
        std::map < std::string, locationConfig > locations;
        //void getconfig();
        void setconfig(const std::vector<std::string> &);
};

class GlobalConfig:protected Keywords{
    private :
       
        std::map < int, std::string > serverNames;
        std::string currentLocation;
        std::string currentServer;
    public :
        std::map <std::string,  std::vector<std::string> > config;
        std::map < std::string, serverConfig > servers;
        void setconfig(const std::vector<std::string> &);
        void setServers(const std::vector<std::string>&);
        const std::map<std::string, std::vector<std::string> >& getGlobalConfig() const;
        bool isAutoindexAllowed( const std::string& currentLocation,
                        const std::string& currentServer) const;
        std::vector<std::string> getDefaultPages( const std::string& currentLocation,
                        const std::string& currentServer) const;
        std::string getRoot( const std::string& currentLocation,
                        const std::string& currentServer) const;
        std::string getLocation(const std::string URI, const std::string& currentServer) const;
        bool isCGIAllowed( const std::string& currentLocation,
                        const std::string& currentServer) const;
        void getconfig();
        GlobalConfig(const char *);
};

template<typename Iterator, typename T2>
bool allOf(Iterator first,Iterator last,T2 fun)
{
    for (Iterator it = first;it != last ; it++)
    {
        if(!fun(*it))
        {
            return (false);
        }
    }
    return (true);
}


#endif