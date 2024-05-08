#include "getMethod.hpp"

string	getMimeTypes(Request &req){
	ifstream file("mime.types");
	if(!file.is_open())
		cerr << "Could not open file";
	string content;
	map<string, string> mime;
	string extention;
	string value;
	while(getline(file, content)){
		if (content.empty())
			continue;
		extention = content.substr(content.find_first_of(' '));
		value = content.substr(0, content.find_first_of(' '));
		int i = 0;
		while(extention[i] == ' ')
			i++;
		extention = &extention[i];
		mime[extention] = value;
	}
	string path = req.getPath().substr(req.getPath().find_last_of('.') + 1);
	for (std::map<std::string, std::string>::const_iterator it = mime.begin(); it != mime.end(); ++it) {
		if (it->first.find(path) != std::string::npos)
			return it->second;
	}
	return "application/octet-stream";
}

string	getStatusCodeMsg(int status){
	if (status == 301)
		return "Moved Permanently";
	if (status == 200)
		return "OK";
	if (status == 404)
		return "Not Found";
	if (status == 500)
		return "Internal Server Error";
	if (status == 501)
		return "Not Implemented";
	if (status == 505)
		return "HTTP Version Not Supported";
	if (status == 201)
		return "Created";
	if (status == 400)
		return "Bad Request";
	if (status == 414)
		return "URI Too Long";
	if (status == 411)
		return "Length Required";
	return "";
}

bool	isDir(const char *path){
	struct stat statbuf;
	if (stat(path, &statbuf) == 0)
    	return S_ISDIR(statbuf.st_mode);
  	else
    	return false;
}


bool hasFiles(const char *dirPath) {
	DIR *dir;
	struct dirent *ent;
	bool has_files = false;
	dir = opendir(dirPath);
	if (dir){
		while ((ent = readdir(dir))){
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
				continue;
			has_files = true;
			break;
		}
		closedir(dir);
	} 
	else
		return false;
	return has_files;
}

void	getMeth(Request &req){
	bool checkDir = false, dirContent = false;
	ifstream File;
	checkDir = isDir(req.getPath().c_str());
	if (!checkDir){
		File.open(req.getPath().c_str());
		if (!File.is_open())
			cerr << "Failed to open file\n";
	}
	else{
		if (req.getPath()[req.getPath().size() - 1] != '/')
			req.status = 301;
		else
			dirContent = hasFiles(req.getPath().c_str());
	}
	cout << req.getPath() << endl;
	string body;
	string statusCode = to_string(req.status);
	string response = req.getVersion() + " " + statusCode + " " + getStatusCodeMsg(req.status) + "\r\n";
	if (req.status == 200){
		getline(File, body, '\0');
		response += "Content-Type: " + getMimeTypes(req) + "\r\n";
	}
	else if (req.status != 301){
		response += "Content-Type: text/html\r\n";
		if (req.status == 404){
			ifstream notFound("error_pages/notfound.html");
			if (!notFound.is_open())
				cerr << "Failed to open file\n";
			getline(notFound, body, '\0');
		}
		if (req.status == 500){
			ifstream serverErr("error_pages/internalservererror.html");
			if (!serverErr.is_open())
				cerr << "Failed to open file\n";
			getline(serverErr, body, '\0'); 
		}
	}
	else
		response += "Location: " + req.getPath().substr(req.getPath().find_last_of('/')) + "/\r\n";
	response += "Connection: close\r\n\n";
	cout  << response << endl;
	send(req.clientSocket, response.c_str(), response.size(), 0);
	const char* data = body.c_str();
    size_t bytesSent = 0;
    size_t dataLength = body.size();
    while (bytesSent < dataLength) {
        int bytesToSend = min<int>(4096, dataLength - bytesSent);
        int sentBytes = send(req.clientSocket, data + bytesSent, bytesToSend, 0);
        if (sentBytes == -1){
            perror("send");
            break;
        }
        bytesSent += sentBytes;
    }
	send(req.clientSocket, "", 1, 0);
	File.close();
	close(req.clientSocket);
}