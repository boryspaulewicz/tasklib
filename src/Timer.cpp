#include "Timer.hpp"

tp Timer::start;
bool Timer::initialized = false;

Timer::Timer(){
    if(!Timer::initialized){
        Timer::start = high_resolution_clock::now();
        Timer::initialized = true;
    }
}