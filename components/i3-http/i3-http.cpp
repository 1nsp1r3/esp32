#include "i3-http.h"

/**
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_client.html#basic-http-request
 * https://github.com/espressif/esp-idf/blob/b3f7e2c8a4d354df8ef8558ea7caddc07283a57b/examples/protocols/esp_http_client/main/esp_http_client_example.c
 */

/**
 * Handler
 */
esp_err_t i3HttpEventHandler(esp_http_client_event_t *Evt){
  ESP_LOGV(I3_HTTP_TAG, "i3HttpEventHandler()");

  static int totalLen = 0;
  int len = 0;

  switch(Evt->event_id) {
    case HTTP_EVENT_ERROR:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_ERROR");
      break;
    case HTTP_EVENT_ON_CONNECTED:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_ON_CONNECTED");
      totalLen = 0;
      memset(Evt->user_data, 0, I3_HTTP_MAX_LENGTH);
      break;
    case HTTP_EVENT_HEADER_SENT:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_HEADER_SENT");
      break;
    case HTTP_EVENT_ON_HEADER:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", Evt->header_key, Evt->header_value);
      break;
    case HTTP_EVENT_ON_DATA:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_ON_DATA, len=%d", Evt->data_len);
      //-1 to keep at least one trailing zero
      len = (totalLen + Evt->data_len) > (I3_HTTP_MAX_LENGTH-1) ? (I3_HTTP_MAX_LENGTH-1) - totalLen : Evt->data_len;
      memcpy(Evt->user_data + totalLen, Evt->data, len);
      totalLen += len;
      break;
    case HTTP_EVENT_ON_FINISH:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_ON_FINISH");
      break;
    case HTTP_EVENT_DISCONNECTED:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_DISCONNECTED");
      break;
    case HTTP_EVENT_REDIRECT:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_REDIRECT");
      break;
    default:
      ESP_LOGV(I3_HTTP_TAG, "i3HttpEventHandler(event: %d)", Evt->event_id);
  }
  return ESP_OK;
}

/**
 * Url: http://worldtimeapi.org/api/timezone/Europe/Paris
 *          ^
 *          found
 *
 * urlNew: worldtimeapi.org/api/timezone/Europe/Paris
 *                         ^
 *                         found2
 */
UrlPart* i3Extract(const char *Url){
  ESP_LOGV(I3_HTTP_TAG, "I3Http::i3Extract(Url: '%s')", Url);

  UrlPart* urlPart = new UrlPart();

  string url = string(Url);

  size_t found = url.find_first_of(":");
  urlPart->protocol = url.substr(0, found);

  string urlNew = url.substr(found+3);
  size_t found2 = urlNew.find_first_of("/");

  urlPart->host = urlNew.substr(0, found2);
  urlPart->path = urlNew.substr(found2);
  return urlPart;
}

/**
 * @return status code
 */
int I3Http::get(const char* Url){
  ESP_LOGV(I3_HTTP_TAG, "I3Http::get(Url: '%s')", Url);

  UrlPart* urlPart = this->i3Extract(Url);
  this->code = 0;
  this->length = 0;

  esp_http_client_config_t config = {
    .host          = urlPart->host.c_str(),
    .path          = urlPart->path.c_str(),
    .query         = "esp", //?
    .event_handler = i3HttpEventHandler,
    .user_data     = this->body,
  };

  esp_http_client_handle_t client = esp_http_client_init(&config);

  esp_err_t err = esp_http_client_perform(client);
  if (err != ESP_OK){
    ESP_LOGE(I3_HTTP_TAG, "Query failed");
    return -1;
  }

  this->code = esp_http_client_get_status_code(client);
  this->length = esp_http_client_get_content_length(client);
  return this->code;
}
