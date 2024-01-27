#include "i3-string.h"

/**
 *
 */
string i3StringReplace(string Source, string Keyword, string With){
    ESP_LOGV(I3_STRING_TAG, "i3StringReplace(Source: '%s', Keyword: '%s', With: '%s')", Source.c_str(), Keyword.c_str(), With.c_str());

    size_t found = Source.find(Keyword);
    if (found == string::npos) return Source;
    Source.replace(
        found,
        Keyword.length(),
        With
    );
    return Source;
}

/**
 * Usage:
 * std::list<string> pair = i3StringSplit("keyword=value", '=');
 * pair.back() -> "value"
 */
list<string> i3StringSplit(string Source, char Delimiter){
    ESP_LOGV(I3_STRING_TAG, "i3StringSplit(Source: '%s', Delimiter: '%c')", Source.c_str(), Delimiter);

    std::list<string> ret;
    size_t found = Source.find(Delimiter);
    if (found == string::npos) return ret;
    ret.push_back(Source.substr(0, found)); //first
    ret.push_back(Source.substr(++found, string::npos)); //second
    return ret;
}
