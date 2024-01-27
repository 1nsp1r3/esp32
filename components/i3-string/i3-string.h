#ifndef I3_STRING_H
#define I3_STRING_H

#define I3_STRING_TAG "I3-STRING"

#include <string>
#include <list>
#include <esp_log.h>

using namespace std;

string i3StringReplace(string Source, string Keyword, string With);
list<string> i3StringSplit(string Source, char Delimiter);

#endif
