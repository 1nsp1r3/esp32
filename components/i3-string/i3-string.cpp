#include "i3-string.h"

/**
 *
 */
string i3StringReplace(string source, string keyword, string with){
    size_t found = source.find(keyword);
    if (found == string::npos) return source;
    source.replace(
        found,
        keyword.length(),
        with
    );
    return source;
}

/**
 * Usage:
 * std::list<string> pair = i3StringSplit("keyword=value", '=');
 * pair.back() -> "value"
 */
list<string> i3StringSplit(string source, char delimiter){
    std::list<string> ret;
    size_t found = source.find(delimiter);
    if (found == string::npos) return ret;
    ret.push_back(source.substr(0, found)); //first
    ret.push_back(source.substr(++found, string::npos)); //second
    return ret;
}
