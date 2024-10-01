#include <Arduino.h>
#include <FastLED.h>
#include <AnimationHelper.h>
#ifndef ANIM
#define ANIM
    struct Animation {
        animPtr anim;
        String name;
    };

    void cylon(void* s);
    void cycle(void* s);
    //void fireworks();
    void fadeall(AnimationHelper* helper, uint8_t dec);
    void halloween(void* s);
    void fall(void* s);
    void setSemaphore(SemaphoreHandle_t* xSem);
    void christmas(void* s);
    void twinkle(void* s);
    void candyCane(void *s);
    void multiSparkle(void *s);
    void chSparkle(void *s);
    //void white(void *s);

    //declare Animation array {void AnimationFunction, String name}
    Animation animations[] = {{cylon, "Cylon"}, 
                              {cycle, "Cycle"},
                              {halloween, "Halloween"}, 
                              {fall, "Fall"},
                              {christmas, "Christmas"},
                              {twinkle, "Twinkle"},
                              {candyCane, "Candy Cane"},
                              {multiSparkle, "Multicolor Sparkle"},
                              {chSparkle, "Christmas Sparkle"}
                              /*,{white, "White"}*/};
    #endif
