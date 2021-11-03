//
// Created by Иван Ильин on 03.11.2021.
//

#include "Timer.h"

using namespace std::chrono;

void Timer::start() {
    _startTime = high_resolution_clock::now();
    _isRunning = true;
}

void Timer::stop() {
    _endTime = high_resolution_clock::now();
    _isRunning = false;
}

double Timer::elapsedMilliseconds() const {
    high_resolution_clock::time_point endTime;

    if(_isRunning) {
        endTime = high_resolution_clock::now();
    } else {
        endTime = _endTime;
    }

    return duration<double>(endTime - _startTime).count();
}

double Timer::elapsedSeconds() const {
    return elapsedMilliseconds() / 1000.0;
}
