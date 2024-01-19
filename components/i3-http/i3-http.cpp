#include "i3-http.h"

/**
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_client.html#basic-http-request
 * https://github.com/espressif/esp-idf/blob/b3f7e2c8a4d354df8ef8558ea7caddc07283a57b/examples/protocols/esp_http_client/main/esp_http_client_example.c
 */

/**
 * Handler
 */
esp_err_t i3HttpEventHandler(esp_http_client_event_t *evt){
  static int totalLen = 0;
  int len = 0;

  switch(evt->event_id) {
    case HTTP_EVENT_ERROR:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_ERROR");
      break;
    case HTTP_EVENT_ON_CONNECTED:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_ON_CONNECTED");
      totalLen = 0;
      memset(evt->user_data, 0, I3_HTTP_MAX_LENGTH);
      break;
    case HTTP_EVENT_HEADER_SENT:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_HEADER_SENT");
      break;
    case HTTP_EVENT_ON_HEADER:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
      break;
    case HTTP_EVENT_ON_DATA:
      ESP_LOGV(I3_HTTP_TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
      //-1 to keep at least one trailing zero
      len = (totalLen + evt->data_len) > (I3_HTTP_MAX_LENGTH-1) ? (I3_HTTP_MAX_LENGTH-1) - totalLen : evt->data_len;
      memcpy(evt->user_data + totalLen, evt->data, len);
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
      ESP_LOGV(I3_HTTP_TAG, "i3HttpEventHandler(event: %d)", evt->event_id);
  }
  return ESP_OK;
}


/**
 * @return status code
 */
int I3Http::get(const char* url){
  ESP_LOGV(I3_HTTP_TAG, "I3Http::get(%s)", url);

  this->code = 0;
  this->length = 0;

  esp_http_client_config_t config = {
    .host          = "worldtimeapi.org",
    .path          = "/api/timezone/Europe/Paris",
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
