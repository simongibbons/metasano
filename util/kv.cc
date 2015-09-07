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
        std::string value(token.begin() + it + 1, token.end());
        ret[key] = value;
    }

    return ret;
}

std::string encode_kv_cookie(const std::map<std::string, std::string> &cookie)
{
    std::stringstream ss;

    for(auto& kv : cookie) {
        ss << kv.first << "=" << kv.second << "&";
    }

    std::string ret = ss.str();
    if(ret.size() > 0 ) ret.erase(ret.size() - 1);
    return ret;
}
