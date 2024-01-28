#include "i3-httpd.h"

GetMap _getMap;
PostMap _postMap;
httpd_handle_t _server = NULL;

/**
 * Start a http server
 * Call espAddPath() to serve some html page
 */
httpd_handle_t* i3HttpdStart(){
  ESP_LOGV(I3_HTTPD_TAG, "i3HttpdStart()");

  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  ESP_LOGI(I3_HTTPD_TAG, "Starting web server on port %d...", config.server_port);
  esp_err_t ret = httpd_start(&_server, &config);
  if (ret != ESP_OK){
    ESP_LOGE(I3_HTTPD_TAG, "Unable to start web server");
    return NULL;
  }
  ESP_LOGI(I3_HTTPD_TAG, "Starting web server on port %d... OK", config.server_port);
  return &_server;
}

/**
 * @private
 */
GetMap::iterator _findGetEndpoint(const char *uri){
  ESP_LOGV(I3_HTTPD_TAG, "_findGetEndpoint(uri: '%s')", uri);

  GetMap::iterator it = _getMap.begin();
  while (it != _getMap.end()){
    if (strcmp(it->first, uri) == 0) return it;
    it++;
  }
  return _getMap.begin();
}

/**
 * @private
 * Process http GET requests
 * Send the html source according to the path
 */
esp_err_t _getHandler(httpd_req_t *Req) {
  ESP_LOGV(I3_HTTPD_TAG, "_getHandler()");
  GetReponse* getResponse;

  ESP_LOGI(I3_HTTPD_TAG, "GET %s", Req->uri);

  GetMap::iterator it = _findGetEndpoint(Req->uri);
  getResponse = it->second();
  httpd_resp_set_type(Req, getResponse->contentType);
  return httpd_resp_send(Req, getResponse->content, getResponse->contentLength); //HTTPD_RESP_USE_STRLEN
}

/**
 * @private
 */
bool _sendPart(httpd_req_t *Req, GetReponse* GetResponse){
  esp_err_t ret;
  char * buffer[64];
  const char* boundary = "\r\n--" MULTIPART_BOUNDARY "\r\n";

  ret = httpd_resp_send_chunk(Req, boundary, strlen(boundary));
  if (ret != ESP_OK) return false;

  size_t length = snprintf(
    (char *)buffer, 64, "Content-Type: %s\r\nContent-Length: %u\r\n\r\n",
    GetResponse->contentType,
    GetResponse->contentLength
  );
  ret = httpd_resp_send_chunk(Req, (const char *)buffer, length);
  if (ret != ESP_OK) return false;

  ret = httpd_resp_send_chunk(Req, GetResponse->content, GetResponse->contentLength);
  if (ret != ESP_OK) return false;

  return true;
}

/**
 * @private
 */
esp_err_t _streamHandler(httpd_req_t *Req) {
  ESP_LOGV(I3_HTTPD_TAG, "_streamHandler()");
  GetReponse* getResponse;

  ESP_LOGI(I3_HTTPD_TAG, "GET %s", Req->uri);

  httpd_resp_set_type(Req, MULTIPART);

  GetMap::iterator it = _findGetEndpoint(Req->uri);
  while(true){ //All other frames
    getResponse = it->second();
    if (!_sendPart(Req, getResponse)) break;
  }
  return ESP_FAIL;
}

/**
 * @private
 * Process http POST requests
 * Send the html source according to the path
 */
esp_err_t _postHandler(httpd_req_t *Req){
  ESP_LOGV(I3_HTTPD_TAG, "_postHandler()");

  ESP_LOGI(I3_HTTPD_TAG, "POST %s", Req->uri);
  ESP_LOGD(I3_HTTPD_TAG, "Content length %d", Req->content_len);

  char* content = (char *)calloc(255, sizeof(char));
  int ret = httpd_req_recv(Req, content, Req->content_len);
  if (ret <= 0){
    if (ret == HTTPD_SOCK_ERR_TIMEOUT) httpd_resp_send_408(Req);
    return ESP_FAIL;
  }

  PostMap::iterator it = _postMap.begin();
  while (it != _postMap.end()){
    if (strcmp(it->first, Req->uri) == 0){
      it->second(content);
      httpd_resp_send(Req, "OK", HTTPD_RESP_USE_STRLEN);
      return ESP_OK;
    }
    it++;
  }
  return ESP_FAIL;
}

/**
 * Associate a html source to a path
 * E.g. "/" -> index.html
 */
void i3HttpdAddGetEndpoint(const char* Path, GetCallBack GetCallBack){
  ESP_LOGV(I3_HTTPD_TAG, "i3HttpdAddGetEndpoint(Path: '%s')", Path);

  _getMap.insert(GetMap::value_type(Path, GetCallBack));
  httpd_uri_t index_uri = {
    .uri       = Path,
    .method    = HTTP_GET,
    .handler   = _getHandler,
    .user_ctx  = NULL
  };
  esp_err_t ret = httpd_register_uri_handler(_server, &index_uri);
  if (ret != ESP_OK){
    ESP_LOGE(I3_HTTPD_TAG, "Unable to add get endpoint %s", Path);
    return;
  }
  ESP_LOGI(I3_HTTPD_TAG, "Get '%s' endpoint added", Path);
}

/**
 *
 */
void i3HttpdAddStreamEndpoint(const char* Path, GetCallBack GetCallBack){
  ESP_LOGV(I3_HTTPD_TAG, "i3HttpdAddStreamEndpoint(Path: '%s')", Path);

  _getMap.insert(GetMap::value_type(Path, GetCallBack));
  httpd_uri_t index_uri = {
    .uri       = Path,
    .method    = HTTP_GET,
    .handler   = _streamHandler,
    .user_ctx  = NULL
  };
  esp_err_t ret = httpd_register_uri_handler(_server, &index_uri);
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
void i3HttpdAddPostEndpoint(const char* Path, PostCallBack PostCallBack){
  ESP_LOGV(I3_HTTPD_TAG, "i3HttpdAddPostEndpoint(Path: '%s')", Path);

  _postMap.insert(PostMap::value_type(Path, PostCallBack));

  httpd_uri_t index_uri = {
    .uri       = Path,
    .method    = HTTP_POST,
    .handler   = _postHandler,
    .user_ctx  = NULL
  };
  esp_err_t ret = httpd_register_uri_handler(_server, &index_uri);
  if (ret != ESP_OK){
    ESP_LOGE(I3_HTTPD_TAG, "Unable to add post endpoint %s", Path);
    return;
  }
  ESP_LOGI(I3_HTTPD_TAG, "Post '%s' endpoint added", Path);
}
