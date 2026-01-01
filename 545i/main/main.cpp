#include <string.h>
#include <time.h>
#include <i3-lcd.h>

#define TAG "I3-MAIN"

#define BALL_SIZE 50

time_t startTime;
char timer[9];

/**
 * Calculate 00:00:00
 */
void processTime(){
    time_t currentTime;
  
    time(&currentTime);
    unsigned int numberOfSeconds = currentTime-startTime;

    unsigned int hours   = numberOfSeconds / 3600;
    unsigned int minutes = (numberOfSeconds % 3600) / 60;
    unsigned int seconds = numberOfSeconds % 60;

    if (hours > 99) hours = 99;
    if (minutes > 59) minutes = 59;
    if (seconds > 59) seconds = 59;

    snprintf(timer, sizeof(timer), "%02u:%02u:%02u", hours, minutes, seconds);
}

/**
 * MAIN
 */
extern "C" void app_main(){
  esp_log_level_set(TAG       , ESP_LOG_VERBOSE); //Log level (ESP_LOG_NONE|ESP_LOG_VERBOSE)
  esp_log_level_set(I3_LCD_TAG, ESP_LOG_VERBOSE);

  ESP_LOGI(TAG, "Bonjour :-)");

  uint16_t *buffer = (uint16_t*)malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
  if (buffer != NULL) ESP_LOGI(TAG, "Initializing back buffer... OK");

  i3LcdInit();

  int x = 120;
  int incX = 1;
  int incY = 1;
  int y = 160;

  time(&startTime);

  char temperature[4];
  sprintf(temperature, "%d", 118);

  for(;;){
    processTime();

    i3LcdClear(buffer);
    i3LcdRectangle(buffer, x, y, x+BALL_SIZE-1, y+BALL_SIZE-1, LCD_COLOR_BLUE);
    
    i3LcdString(buffer, 2, 0, timer, LCD_COLOR_GREEN, 4);
    i3LcdString(buffer, 10, 100, temperature, LCD_COLOR_WHITE, 10);

    i3LcdString(buffer, 2, 305, "Inspir3 (c) 2026", LCD_COLOR_BLUE, 1);

    i3LcdSwap(buffer);

    x+=incX;
    y+=incY;

    if (x >= LCD_WIDTH-BALL_SIZE) incX = -1;
    if (x <= 0) incX = 1;
    if (y >= LCD_HEIGHT-BALL_SIZE) incY = -1;
    if (y <= 0) incY = 1;
  }
}
