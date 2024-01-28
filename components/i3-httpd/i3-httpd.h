#ifndef I3_HTTPD_H
#define I3_HTTPD_H

#define I3_HTTPD_TAG "I3-HTTPD"

#include <map>
#include <esp_log.h>
#include <esp_http_server.h>

/**
 *
 */
class I3HttpdContent {
  public:
    char type[20]; // "text/html" ou "image/jpeg"
    size_t length;
    const char* data;
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
typedef I3HttpdContent* (*GetCallBack)();

typedef std::map<const char*, GetCallBack> GetMap;
typedef std::map<const char*, PostCallBack> PostMap;


esp_err_t _getHandler(httpd_req_t* Req);
esp_err_t _postHandler(httpd_req_t* Req);

class I3Httpd {
  protected:
    httpd_handle_t server = NULL;

  public:
    static I3Httpd* instance;
    GetMap getMap;
    PostMap postMap;

    I3Httpd(){ I3Httpd::instance = this; }
    void start();
    void addGetEndpoint(const char* Path, GetCallBack GetCallBack);
    void addPostEndpoint(const char* Path, PostCallBack PostCallBack);
    GetMap::iterator findGetFromMap(const char* uri);
    PostMap::iterator findPostFromMap(const char* uri);
};

#endif
