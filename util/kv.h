#pragma once

#include <map>
#include <string>

std::map< std::string, std::string > parse_kv_cookie(const std::string &s);
