#pragma once

#include "cgi.hpp"
#include "config.hpp"
#include "message.hpp"
#include "request.hpp"
#include "response.hpp"

using namespace std;
class Request;

class GET{
	public:
		int	client_socket;
};
void	getMeth(Request &req);