#include "i3-httpd.h"

I3Httpd* I3Httpd::instance = NULL;

/**
 * Start a http server
 */
void I3Httpd::start(){
  ESP_LOGV(I3_HTTPD_TAG, "I3Httpd::start()");

  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  ESP_LOGI(I3_HTTPD_TAG, "Starting web server on port %d...", config.server_port);
  esp_err_t ret = httpd_start(&this->server, &config);
  if (ret != ESP_OK){
    ESP_LOGE(I3_HTTPD_TAG, "Unable to start web server");
    return;
  }
  ESP_LOGI(I3_HTTPD_TAG, "Starting web server on port %d... OK", config.server_port);
}

/**
 * Associate a html source to a path
 * E.g. "/" -> index.html
 */
void I3Httpd::addGetEndpoint(const char* Path, GetCallBack GetCallBack){
  ESP_LOGV(I3_HTTPD_TAG, "I3Httpd::addGetEndpoint(Path: '%s')", Path);

  this->getMap.insert(GetMap::value_type(Path, GetCallBack));
  httpd_uri_t index_uri = {
    .uri       = Path,
    .method    = HTTP_GET,
    .handler   = _getHandler,
    .user_ctx  = NULL
  };
  esp_err_t ret = httpd_register_uri_handler(this->server, &index_uri);
  if (ret != ESP_OK){
    ESP_LOGE(I3_HTTPD_TAG, "Unable to add get endpoint %s", Path);
    return;
  }
  ESP_LOGI(I3_HTTPD_TAG, "Get '%s' endpoint added", Path);
}

/**
 * Associate a callback to a path
 * E.g. "/action" -> processAction(const char* data)
 */
void I3Httpd::addPostEndpoint(const char* Path, PostCallBack PostCallBack){
  ESP_LOGV(I3_HTTPD_TAG, "I3Httpd::addPostEndpoint(Path: '%s')", Path);

  this->postMap.insert(PostMap::value_type(Path, PostCallBack));

  httpd_uri_t index_uri = {
    .uri       = Path,
    .method    = HTTP_POST,
    .handler   = _postHandler,
    .user_ctx  = NULL
  };
  esp_err_t ret = httpd_register_uri_handler(this->server, &index_uri);
  if (ret != ESP_OK){
    ESP_LOGE(I3_HTTPD_TAG, "Unable to add post endpoint %s", Path);
    return;
  }
  ESP_LOGI(I3_HTTPD_TAG, "Post '%s' endpoint added", Path);
}

/**
 * @protected
 */
GetMap::iterator I3Httpd::findGetFromMap(const char* uri){
  ESP_LOGV(I3_HTTPD_TAG, "I3Httpd::findGetFromMap(uri: '%s')", uri);

  GetMap::iterator it = this->getMap.begin();
  while (it != this->getMap.end()){
    if (strcmp(it->first, uri) == 0) return it;
    it++;
  }
  return this->getMap.begin();
}

/**
 * @protected
 */
PostMap::iterator I3Httpd::findPostFromMap(const char* uri){
  ESP_LOGV(I3_HTTPD_TAG, "I3Httpd::findPostFromMap(uri: '%s')", uri);

  PostMap::iterator it = this->postMap.begin();
  while (it != this->postMap.end()){
    if (strcmp(it->first, uri) == 0) return it;
    it++;
  }
  return this->postMap.begin();
}

/**
 * @private
 * Process http GET requests
 * Send the html source according to the path
 *
 * E.g.
 * Content-Type: image/jpeg
 * Content-Length: 115563
 * [data...]
 */
esp_err_t _getHandler(httpd_req_t* Req) {
  ESP_LOGV(I3_HTTPD_TAG, "_getHandler()");
  I3HttpdContent* content;
  I3Httpd* that = I3Httpd::instance;

  ESP_LOGI(I3_HTTPD_TAG, "GET %s", Req->uri);

  GetMap::iterator it = that->findGetFromMap(Req->uri);
  content = it->second();
  httpd_resp_set_type(Req, content->type);
  return httpd_resp_send(Req, content->data, content->length);
}

/**
 * @private
 * Process http POST requests
 * Send the html source according to the path
 */
esp_err_t _postHandler(httpd_req_t* Req){
  ESP_LOGV(I3_HTTPD_TAG, "_postHandler()");
  I3Httpd* that = I3Httpd::instance;
  char* content = (char*)calloc(255, sizeof(char));

  ESP_LOGI(I3_HTTPD_TAG, "POST %s", Req->uri);
  ESP_LOGD(I3_HTTPD_TAG, "Content length %d", Req->content_len);

  esp_err_t ret = httpd_req_recv(Req, content, Req->content_len);
  if (ret != ESP_OK){
    if (ret == HTTPD_SOCK_ERR_TIMEOUT) httpd_resp_send_408(Req);
    return ESP_FAIL;
  }

  PostMap::iterator it = that->findPostFromMap(Req->uri);
  it->second(content);
  httpd_resp_send(Req, "OK", HTTPD_RESP_USE_STRLEN);
  return ESP_OK;
}
