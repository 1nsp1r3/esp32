#ifndef I3_HTTP_H
#define I3_HTTP_H

#define I3_HTTP_TAG "I3-HTTP"
#define I3_HTTP_MAX_LENGTH 300

#include <string>
#include <esp_log.h>
#include <esp_http_client.h>

using namespace std;

class UrlPart {
  public:
    string protocol;
    string host;
    string path;
};

class I3Http {
  public:
    char body[I3_HTTP_MAX_LENGTH];
    int length;
    int code;

    int get(const char* Url);

  protected:
    UrlPart* i3Extract(const char *Url);
};

#endif
