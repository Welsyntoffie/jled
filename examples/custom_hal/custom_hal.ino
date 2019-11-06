// JLed custom HAL example.
// Copyright 2019 by Jan Delgado. All rights reserved.
// https://github.com/jandelgado/jled

// we include jled_base.h instead of "jled.h" since we define our own JLed
// class using our custom HAL.
#include <jled_base.h>

// a custom HAL for the Arduino, inverting output and ticking with half
// the speed.
class CustomHal : jled::JLedHal {
 private:
    template <typename T, typename B>
    friend class jled::TJLed;
    CustomHal() {}

 public:
    explicit CustomHal(uint8_t pin) noexcept : pin_(pin) {}

    void analogWrite(uint8_t val) const {
        // some platforms, e.g. STM need lazy initialization
        if (!setup_) {
            ::pinMode(pin_, OUTPUT);
            setup_ = true;
        }
        ::analogWrite(pin_, 255 - val);
    }

    uint32_t millis() const { return ::millis() >> 1; }

 private:
    mutable bool setup_ = false;
    uint8_t pin_;
};

class JLed : public jled::TJLed<CustomHal, JLed> {
    using jled::TJLed<CustomHal, JLed>::TJLed;
};

// uses above defined CustomHal
auto led = JLed(LED_BUILTIN).Blink(1000, 1000).Repeat(5);

void setup() {}

void loop() { led.Update(); }
