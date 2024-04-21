#include"response.hpp"

Response::Response() {
    code[200] = "OK";
    code[404] = "Not Found";
    code[500] = "Internal Server Error";
    code[501] = "Not Implemented";
    code[505] = "HTTP Version Not Supported";
    code[201] = "Created";
    code[400] = "Bad Request";
    code[414] = "URI Too Long";
    code[411] = "Length Required";
}


