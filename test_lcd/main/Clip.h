#ifndef I3_CLIP_H
  #define I3_CLIP_H

/**
 *
 */
class Clip {
  public:
    char *name = NULL;
    bool isFinished;

    Clip(char* Name){
      this->name = Name;
      this->isFinished = false;
    }

    virtual void reset(){
    }

    virtual void playFrame(){
      this->isFinished = true;
    }
};

#endif