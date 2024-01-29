#include "i3-stream-server.h"

/**
 *
 */
bool I3StreamServer::sendPart(httpd_req_t* Req, I3HttpdContent* Content){
  ESP_LOGV(I3_STREAM_SERVER_TAG, "I3StreamServer::sendPart()");
  esp_err_t ret;
  char buffer[64];
  const char* boundary = "\r\n--" MULTIPART_BOUNDARY "\r\n";

  ret = httpd_resp_send_chunk(Req, boundary, strlen(boundary));
  if (ret != ESP_OK) return false;

  size_t length = snprintf(
    buffer, 64, "Content-Type: %s\r\nContent-Length: %u\r\n\r\n",
    Content->type,
    Content->length
  );
  ret = httpd_resp_send_chunk(Req, buffer, length);
  if (ret != ESP_OK) return false;

  ret = httpd_resp_send_chunk(Req, Content->data, Content->length);
  if (ret != ESP_OK) return false;

  return true;
}

/**
 * @private
 *
 * E.g.
 * Content-Type: multipart/x-mixed-replace;boundary=Inspir3
 *
 * --Inspir3
 * Content-Type: image/jpeg
 * Content-Length: 119230
 *
 * [data...]
 *
 * --Inspir3
 * Content-Type: image/jpeg
 * Content-Length: 119230
 *
 * [data...]
 */
esp_err_t _streamHandler(httpd_req_t* Req) {
  ESP_LOGV(I3_STREAM_SERVER_TAG, "_streamHandler()");
  I3HttpdContent* content;
  I3StreamServer* that = (I3StreamServer*)I3StreamServer::instance;

  ESP_LOGI(I3_STREAM_SERVER_TAG, "GET %s", Req->uri);

  httpd_resp_set_type(Req, MULTIPART);

  GetMap::iterator it = that->findGetFromMap(Req->uri);
  while(true){ //All other frames
    content = it->second();
    if (!that->sendPart(Req, content)) break;
  }
  return ESP_FAIL;
}

/**
 *
 */
void I3StreamServer::addStreamEndpoint(const char* Path, GetCallBack GetCallBack){
  ESP_LOGV(I3_STREAM_SERVER_TAG, "I3StreamServer::addStreamEndpoint(Path: '%s')", Path);

  this->getMap.insert(GetMap::value_type(Path, GetCallBack));
  httpd_uri_t index_uri = {
    .uri       = Path,
    .method    = HTTP_GET,
    .handler   = _streamHandler,
    .user_ctx  = NULL
  };
  esp_err_t ret = httpd_register_uri_handler(this->server, &index_uri);
  if (ret != ESP_OK){
    ESP_LOGE(I3_STREAM_SERVER_TAG, "Unable to add stream endpoint %s", Path);
    return;
  }
  ESP_LOGI(I3_STREAM_SERVER_TAG, "Get '%s' endpoint added", Path);
}
