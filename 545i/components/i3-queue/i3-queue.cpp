#include "i3-queue.h"

/**
 * Constructor (FIFO)
 */
I3Queue::I3Queue(int Size){
    this->size = Size;
    this->clear();
}

/**
 * Clear the queue
 */
void I3Queue::clear(){
    this->count = 0;
    for(int i=0;i<this->size;i++) this->queue[i] = 0;
}

/**
 * Example with a size of 4
 *  1
 *  1 2
 *  1 2 3
 *  1 2 3 4
 *  2 3 4 5
 *  3 4 5 6
 */
void I3Queue::push(float Value){
    if (this->count >= this->size){
        this->shift();
        this->count--;
    }
    this->queue[this->count++] = Value;
}

/**
 * Example with a size of 4
 * 1 2 3 4
 * 2 3 4 ?
 */
void I3Queue::shift(){
    for(int i=0;i<this->size-1;i++){
        this->queue[i] = this->queue[i+1];
    }
}

/**
 * Return the average value of the queue content
 */
float I3Queue::avg(){
    if (!this->count) return 0;

    float sum = 0;
    for(int i=0;i<this->size;i++){
        sum += this->queue[i];
    }
    return sum / this->count;
}

/**
 *
 */
float I3Queue::min(){
    if (!this->count) return 0;

    float min = this->queue[0];
    for(int i=0;i<this->size;i++){
        if (this->queue[i] < min){
            min = this->queue[i];
        }
    }
    return min;
}

/**
 *
 */
float I3Queue::max(){
    if (!this->count) return 0;

    float max = this->queue[0];
    for(int i=0;i<this->size;i++){
        if (this->queue[i] > max){
            max = this->queue[i];
        }
    }
    return max;
}

/**
 *
 */
int I3Queue::length(){
    return this->count;
}

/**
 *
 */
void I3Queue::debug(){
    for(int i=0;i<this->size;i++){
        ESP_LOGI(I3_QUEUE_TAG, "%.2f", this->queue[i]);
    }
    ESP_LOGI(I3_QUEUE_TAG, "Average value: %.2f", this->avg());
}
