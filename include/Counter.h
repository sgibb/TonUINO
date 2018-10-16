#ifndef COUNTER_H
#define COUNTER_H

#include "debug.h"

class Counter {
public:
    Counter(const byte times, const uint16_t maxDuration) :
        _times(times), _maxDuration(maxDuration), _last(0), _counter(0) {};

    bool isFinished() { return _times <= _counter; };
    void trigger();
    void reset();

private:
    const byte _times;
    const uint16_t _maxDuration;
    uint16_t _last;
    uint16_t _counter;
};

void Counter::reset() {
    DPRINTLNF("Reset counter.");
    _last = millis();
    _counter = 0;
}

void Counter::trigger() {
    DPRINTLNF("Trigger counter.");
    uint16_t now = millis();
    uint16_t duration = now - _last;
    DVPRINTLNF(duration);
    _last = now;

    if (duration <= _maxDuration) {
        _counter++;
        DVPRINTLNF(_counter);
        DPRINT(_times - _counter);
        DPRINTLNF(" trigger events left.")
    } else {
        reset();
    }
}

#endif /* !COUNTER_H */
