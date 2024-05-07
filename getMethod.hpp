#pragma once

#include "cgi.hpp"
#include "config.hpp"
#include "message.hpp"
#include "request.hpp"
#include <algorithm>

using namespace std;
class Request;

void	getMeth(Request &req);