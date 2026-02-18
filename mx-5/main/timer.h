#include <time.h>

char timer[9];
time_t startTime;

/**
 *
 */
void timerInit(){
  time(&startTime);
}

/**
 * Generate a timer "00:00:00"
 */
void timerProcess(){
  time_t currentTime;

  time(&currentTime);
  unsigned int numberOfSeconds = currentTime - startTime;

  unsigned int hours = numberOfSeconds / 3600;
  unsigned int minutes = (numberOfSeconds % 3600) / 60;
  unsigned int seconds = numberOfSeconds % 60;

  if (hours > 99) hours = 99;
  if (minutes > 59) minutes = 59;
  if (seconds > 59) seconds = 59;

  snprintf(timer, sizeof(timer), "%02u:%02u:%02u", hours, minutes, seconds);
}
