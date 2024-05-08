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

void	getMeth(Request &req);