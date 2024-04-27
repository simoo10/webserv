#include <iostream>
#include <unistd.h>
#include <cstring>
int main() {
    // CGI script path
    const char* cgiScript = "cgi/script.php";
    char **env = new char*[4];

    // Set environment variables
    env[0] = strdup("SCRIPT_FILENAME=cgi/script.php");
    env[1] = strdup("REQUEST_METHOD=GET");
    env[2] = strdup("REDIRECT_STATUS=200");
    env[3] = NULL;
    char **v = new char*[3];
    v[0] = strdup("/usr/bin/php-cgi");
    v[1] = strdup(cgiScript);
    v[2] = NULL;

    if (execve(v[0], v, env) == -1) {
        std::cerr << "Error: Failed to execute CGI script" << std::endl;
        return 1; // Return error code
    }

    // execve replaces the current process if successful,
    // so this line will only be reached if an error occurs
    return 0;
}