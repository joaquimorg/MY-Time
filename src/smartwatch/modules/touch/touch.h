#ifndef TOUCH_H
#define TOUCH_H

#include "stdint.h"
#include "nrf52.h"
class Touch
{  

  public:

    enum class Gestures : uint8_t {
      None = 0x00,
      SlideDown = 0x01,
      SlideUp = 0x02,
      SlideLeft = 0x03,
      SlideRight = 0x04,
      SingleTap = 0x05,
      DoubleTap = 0x0B,
      LongPress = 0x0C
    };

    Touch(void);

    void init(void);
    void sleep(bool state);
    void read();
    void get(void);

    uint16_t getX(void) { return xpos; }
    uint16_t getY(void) { return ypos; }
    uint16_t getLastX(void) { return last_xpos; }
    uint16_t getLastY(void) { return last_ypos; }
    Gestures getGesture(void) { return gesture; }
    uint8_t getEvent(void) { return event; }
    uint8_t getVersion(void) { return version15; }

    void cleanGesture(void) { gesture = Gestures::None; }

  protected:

    byte data_raw[8];

    Gestures gesture = Gestures::None;
    uint8_t touchpoints;
    uint8_t event;
    uint16_t xpos;
    uint16_t ypos;
    uint16_t last_xpos;
    uint16_t last_ypos;
    uint8_t version15;
    uint8_t versionInfo[3];
};

#endif //TOUCH_H