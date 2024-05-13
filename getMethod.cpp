#include "getMethod.hpp"

string	getMimeTypes(string path){
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
	string ext = path.substr(path.find_last_of('.') + 1);
	for (std::map<std::string, std::string>::const_iterator it = mime.begin(); it != mime.end(); ++it) {
		if (it->first.find(ext) != std::string::npos)
			return it->second;
	}
	return "application/octet-stream";
}

string	getStatusCodeMsg(int status){
	if (status == 301)
		return "Moved Permanently";
	if (status == 200)
		return "OK";
	if (status == 204)
		return "No Content";
	if (status == 403)
		return "Forbidden";
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
	if (status == 409)
		return "Conflict";
	if (status == 414)
		return "URI Too Long";
	if (status == 411)
		return "Length Required";
	return "";
}

int	isDir(const char *path){
	struct stat statbuf;
	if (lstat(path, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
    		return 1;
  		else if (S_ISREG(statbuf.st_mode))
    		return 2;
	}
	return 0;
}


bool hasFiles(const char *dirPath){
	DIR *dir;
	struct dirent *ent;
	bool has_files = false;
	dir = opendir(dirPath);
	if (dir){
		while (ent = readdir(dir)){
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

string	hasIndexFile(const char *dirPath){
	DIR *dir;
	struct dirent *ent;
	vector<string> filenames;
	dir = opendir(dirPath);
	while (ent = readdir(dir)){
		filenames.push_back(ent->d_name);
	}
	closedir(dir);
	for(const string filename : filenames){
		string filename_lowcase = filename;
		transform(filename_lowcase.begin(), filename_lowcase.end(), filename_lowcase.begin(), ::tolower);
		if (filename_lowcase == "index.html" || filename_lowcase == "index.htm")
			return filename_lowcase;
	}
	return "";
}

string	toHex(int size){
	stringstream toHex;
	toHex << hex << size;
	return toHex.str();
}

void	getMeth(Request &req){
	int checkDir = 0;
	bool dirContent = false;
	ifstream File;
	string body;
	string indexFile = "";
	checkDir = isDir(req.getPath().c_str());
	if (checkDir == 2){
		File.open(req.getPath().c_str());
		if (!File.is_open())
			cerr << "Failed to open file\n";
		else
			req.status = 200;
	}
	else if (checkDir == 1){
		if (req.getPath()[req.getPath().size() - 1] != '/')
			req.status = 301;
		else if (req.getPath()[req.getPath().size() - 1] == '/'){
			dirContent = hasFiles(req.getPath().c_str());
			if (dirContent){
				indexFile = hasIndexFile(req.getPath().c_str());
				if (!indexFile.empty()){
					File.open(req.getPath().c_str() + indexFile);
					if (!File.is_open())
						cerr << "Failed to open file\n";
				}
			}
			else
				req.status = 404;
		}
	}
	else if (checkDir == 3)
		cout << "Invalid File or Directory\n";
	cout << req.getPath() << endl;
	string statusCode = to_string(req.status);
	string response = req.getVersion() + " " + statusCode + " " + getStatusCodeMsg(req.status) + "\r\n";
	if (req.status == 200){
		getline(File, body, '\0');
		if (!indexFile.empty())
			response += "Content-Type: " + getMimeTypes(indexFile) + "\r\n";
		else
			response += "Content-Type: " + getMimeTypes(req.getPath()) + "\r\n";
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
	response += "Transfer-Encoding: chunked\r\n";
	response += "Connection: close\r\n\n";
	cout  << response << endl;
	const char* data = body.c_str();
    size_t bytesSent = 0;
    size_t dataLength = body.size();
	send(req.clientSocket, response.c_str(), response.size(), 0);
	while (bytesSent < dataLength){
		int bytesToSend = min<int>(1024, dataLength - bytesSent);
		std::string chunkSize = toHex(bytesToSend);
		send(req.clientSocket, chunkSize.c_str(), chunkSize.size(), 0);
		send(req.clientSocket, "\r\n", 2, 0);
		int totalSent = 0;
		while (totalSent < bytesToSend){
			int sentBytes = send(req.clientSocket, data + bytesSent + totalSent, bytesToSend - totalSent, 0);
			if (sentBytes == -1){
				perror("send");
				break;
			}
			totalSent += sentBytes;
		}
		bytesSent += bytesToSend;
	}
	File.close();
	send(req.clientSocket, "", 1, 0);
	close(req.clientSocket);
}

void	deleteMeth(Request &req){
	int checkDir = isDir(req.getPath().c_str());
	int	deleted = -1;
	if (checkDir == 2)
		deleted = remove(req.getPath().c_str());
	else if (checkDir == 1){
		if (req.getPath()[req.getPath().size() - 1] != '/')
			req.status = 409;
		else{
			DIR *dir;
			struct dirent *ent;
			dir = opendir(req.getPath().c_str());
			while(ent = readdir(dir)){
				if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
					ent = readdir(dir);
				deleted = remove((req.getPath() + ent->d_name).c_str());
				if (deleted == -1)
					break ;
			}
		}
	}
	else
		req.status = 404;
	if (deleted == 0){
		req.status = 204;
		string response;
		response = req.getVersion() + " " + to_string(req.status) + " " + getStatusCodeMsg(req.status) + "\r\n\n";
		ifstream File;
		File.open("error_pages/deleted.html");
		if (!File.is_open())
			cerr << "Failed to open file\n";
		string body;
		getline(File, body, '\0');
		send(req.clientSocket, response.c_str(), response.size(), 0);
		send(req.clientSocket, body.c_str(), body.size(), 0);
		send(req.clientSocket, "", 1, 0);
		close(req.clientSocket);
	}
}