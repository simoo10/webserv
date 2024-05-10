#pragma once

#include "cgi.hpp"
#include "config.hpp"
#include "message.hpp"
#include "request.hpp"
#include <algorithm>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

class Request;

class Get{
	private:
		bool flagResponse;
	public:
		Get();
		bool	getResponseFlag();
		void	setResponseFlag(bool flag);
};

void	getMeth(Request &req);