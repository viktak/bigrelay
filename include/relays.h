#ifndef RELAYS_H
#define RELAYS_H

#include <Arduino.h>

namespace relays
{
    static bool relays[8];

    extern void SwitchRelay(const size_t relay, const bool isOn);
    extern void setup();
    extern void loop();
}

#endif