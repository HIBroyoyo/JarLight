#include "AnimationHelper.h"

AnimationHelper::AnimationHelper(int n, uint8_t p) {
    NLEDS = n;
    pin = p;
}
void AnimationHelper::begin() {
    strip = new NeoPixelBrightnessBus<PIXELTYPE, PIXELSPEED>(NLEDS, pin);
    strip->Begin();
    strip->SetBrightness(brightness);
    strip->Show();
    setColor(100, 100, 100);
}
void AnimationHelper::setColor(uint8_t r, uint8_t g, uint8_t b, bool sho) {
    color = RgbColor(r, g, b);
    if(sho) showColor();
}
#ifdef RGBW 
void AnimationHelper::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w, bool sho) {
    color = RgbwColor(r, g, b, w);
    if(sho) showColor();
}
#endif
void AnimationHelper::setColorHsv(uint8_t h, uint8_t s, uint8_t v, bool sho) {
    color = HslColor(h, s, v);
    if(sho) showColor();
}
void AnimationHelper::setColor(uint32_t c, bool sho) {
    color = c;
    if(sho) showColor();
}
void AnimationHelper::showColor() 
{
if(animation != -1)setAnimation(-1);
fill(colorToUInt32(color));
if(!power) return;
strip->Show();
}
void AnimationHelper::addAnimation(String* name, animPtr anim) {
    Serial.println(numAnims);
    if(animations != NULL) {
        animPtr* oldAnims = animations;
        String** oldNames = animNames;
        animations = new animPtr[++numAnims];
        animNames = new String*[numAnims];
        for(int i = 0; i < numAnims - 1; i++) {
            animations[i] = oldAnims[i];
            animNames[i] = oldNames[i];
        }
        animations[numAnims - 1] = anim;
        animNames[numAnims - 1] = name;
        delete oldAnims;
        delete oldNames;
    }
    else {
        animations = new animPtr[++numAnims];
        animNames = new String*[numAnims];
        animations[0] = anim;
        animNames[0] = name;
    }
}
void AnimationHelper::setAnimation(int a) {
    animation = a;
        if(currentAnimation != NULL) 
        {
            xSemaphoreTake(xSemaphore, portMAX_DELAY);
            vTaskDelete(currentAnimation);
            xSemaphoreGive(xSemaphore);
            Serial.println("delete");
        }
        if(a != -1) {
            #if defined(ESP32DEV) || defined(ESP32S3)
            xTaskCreatePinnedToCore(
                animations[a],
                "Animation Task",
                2048,
                this,
                2,
                &currentAnimation,
                1);
            #endif
            #ifdef ESP32C3
            xTaskCreate(
                animations[animation],
                "Animation Task",
                2048,
                this,
                2,
                NULL);
            #endif
            #ifdef ESP32S2
            xTaskCreate(
                animations[animation],
                "Animation Task",
                2048,
                this,
                2,
                NULL);
            #endif
        }
    setPower(power);
}
void AnimationHelper::setBrightness(byte b) {
    brightness = b;
    strip->SetBrightness(brightness);
    if(animation == -1 && power) showColor();
}
void AnimationHelper::setSpeed(byte s) {
    speed = (float)s / 255.0;
}
void AnimationHelper::setPower(bool p) {
    power = p;
    if(p) 
    {
        if(animation == -1) showColor();
        else 
        {
            if(currentAnimation != NULL) vTaskResume(currentAnimation);
        }
    }
    else 
    {
        if(animation != -1) 
        {
            if(currentAnimation != NULL) 
            {
                xSemaphoreTake(xSemaphore, portMAX_DELAY);
                vTaskSuspend(currentAnimation);
                xSemaphoreGive(xSemaphore);
            }
        }
        fill(0);
        strip->Show();
    }
}
void AnimationHelper::powerOn() {
    setPower(true);
}
void AnimationHelper::powerOff() {
    setPower(false);
}
void AnimationHelper::setAnimationSemaphore(semaPtr s) {
    s(&xSemaphore);
}

void AnimationHelper::setPixelColor(int p, uint8_t r, uint8_t g, uint8_t b, bool sho) {
    strip->SetPixelColor(p, RgbColor(r, g, b));
    if(sho) show();
}
void AnimationHelper::setPixelColorHsv(int p, uint8_t h, uint8_t s, uint8_t v, bool sho) {
    strip->SetPixelColor(p, HsbColor((float)h/255.0, (float)s/255.0, (float)v/255.0));
    if(sho) show();
}
void AnimationHelper::setPixelColor(int p, uint32_t c, bool sho) {
    strip->SetPixelColor(p, HtmlColor(c));
    if(sho) show();
}
void AnimationHelper::show() {
    strip->Show();
    vTaskDelay(5);
}
void AnimationHelper::setPrimeAnimColor(byte r, byte g, byte b) {
    primaryAnimColor = ((uint32_t)r << 16) + ((uint32_t)g << 8) + (b);
}
    
void AnimationHelper::setSecAnimColor(byte r, byte g, byte b) {
    secondaryAnimColor = ((uint32_t)r << 16) + ((uint32_t)g << 8) + (b);
}
uint32_t AnimationHelper::getPrimeAnimColor() {
    return primaryAnimColor;
}
    
uint32_t AnimationHelper::getSecAnimColor() {
    return secondaryAnimColor;
}
uint32_t AnimationHelper::getPixelColor(int p) {
    return colorToUInt32(strip->GetPixelColor(p));
}

void AnimationHelper::fill(uint32_t c) {
    for(int i=0; i<NUMLEDS; i++)
        #ifdef RGBW
        strip->SetPixelColor(i, RgbwColor(c));
        #else
        strip->SetPixelColor(i, HtmlColor(c));
        #endif
}



bool AnimationHelper::getPower() {
    return power;
}
byte AnimationHelper::getBrightness() {
    return brightness;
}
float AnimationHelper::getSpeed() {
    return speed;
}
int AnimationHelper::getAnimation() {
    return animation;
}
int AnimationHelper::pixelCount() {
    return NUMLEDS;
}
String** AnimationHelper::getAnimationNames() {
    return animNames;
}
int AnimationHelper::getNumberAnimations() {
    return numAnims;
}
uint32_t AnimationHelper::getColor() {
    return colorToUInt32(color);
}
NeoPixelBrightnessBus<PIXELTYPE, PIXELSPEED>* AnimationHelper::getStrip() {
    return strip;
}
void AnimationHelper::setStrip(NeoPixelBrightnessBus<PIXELTYPE, PIXELSPEED>* s) {
    strip = s;   
}
#ifdef RGBW
uint32_t AnimationHelper::colorToUInt32(RgbwColor c) {
    uint32_t temp = c.W << 24;
    temp += c.R << 16;
    temp += c.G << 8;
    temp += c.B;
    return temp;
}
#else
uint32_t AnimationHelper::colorToUInt32(RgbColor c) {
    uint32_t temp = c.R << 16;
    temp += c.G << 8;
    temp += c.B;
    return temp;
}
#endif