#include "kv.h"

#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <exception>

class ParseError : std::runtime_error {
public:
    ParseError(const std::string& s) : 
        std::runtime_error(s)
    {}
};

std::map< std::string, std::string > parse_kv_cookie(const std::string &s)
{
    std::stringstream ss(s);
    std::string token;
    std::map< std::string, std::string > ret;

    while(std::getline(ss, token, '&')) {
        if( std::count(token.begin(), token.end(), '=') != 1 ) {
            throw ParseError("Cannot parse '" + token + "' into key-value tuple");
        }
        auto it = token.find('=');
        std::string key(token.begin(), token.begin() + it);
        std::string value(token.begin() + it, token.end());
        ret[key] = value;
    }

    return ret;
}
