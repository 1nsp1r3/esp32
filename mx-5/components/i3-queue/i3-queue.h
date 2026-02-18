#ifndef I3_QUEUE_H
#define I3_QUEUE_H

#define I3_QUEUE_TAG "I3-QUEUE"

#include <esp_log.h>

class I3Queue {
    private:
      float queue[10];
      int size;
      int count;

     public:
        I3Queue(int Size);
        void clear();
        void push(float Value);
        void shift();
        void debug();
        float avg();
        float min();
        float max();
        int length();
};

#endif
