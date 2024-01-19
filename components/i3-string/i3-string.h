#ifndef I3_STRING_H
#define I3_STRING_H

#include <string>
#include <list>

using namespace std;

string i3StringReplace(string source, string keyword, string with);
list<string> i3StringSplit(string source, char delimiter);

#endif
