#ifndef I3_HTTPD_H
#define I3_HTTPD_H

#define I3_HTTPD_TAG "I3-HTTPD"

#include <map>
#include <esp_log.h>
#include <esp_http_server.h>

#define MULTIPART_BOUNDARY "123456789000000000000987654321"
#define MULTIPART "multipart/x-mixed-replace;boundary=" MULTIPART_BOUNDARY

class GetReponse {
  public:
    char contentType[70]; // "text/html" ou "image/jpeg"
    const char *content;
    size_t contentLength;
};

/**
 * Permet de rattacher une callback de traitement à un endpoint POST /path
 * La callback recoit du texte en entrée et ne retourne rien
 */
typedef void (*PostCallBack)(const char*);

/**
 * Permet de rattacher une callback de traitement à un endpoint GET /path
 * La callback doit simplement fournir du texte (html) en sortie
 */
typedef GetReponse* (*GetCallBack)();

typedef std::map<const char*, GetCallBack> GetMap;
typedef std::map<const char*, PostCallBack> PostMap;

httpd_handle_t* i3HttpdStart();
void i3HttpdAddGetEndpoint(const char* Path, GetCallBack GetCallBack);
void i3HttpdAddStreamEndpoint(const char* Path, GetCallBack GetCallBack);
void i3HttpdAddPostEndpoint(const char* Path, PostCallBack PostCallBack);

#endif
