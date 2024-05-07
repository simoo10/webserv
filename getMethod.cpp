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

void	getMeth(Request &req){
	ifstream File(req.getPath().c_str());
	string body;
	if (!File.is_open())
		cerr << "Failed to open file\n";
	getline(File, body, '\0');
	string statusCode = to_string(req.status);
	string response = req.getVersion() + " " +statusCode + " " + getStatusCodeMsg(req.status) + "\r\n";
	response += "Content-Type: " + getMimeTypes(req) + "\r\n\n";
	response += body;
	cout  << response << endl;
}