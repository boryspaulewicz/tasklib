#include "Timer.hpp"

tp Timer::start;
bool Timer::initialized = false;

void Timer::init() {
    if(!initialized){
        start = high_resolution_clock::now();
        initialized = true;
    }
}

/**
 * Inicjalizacja zegara, wywoływana niejawnie przez konstruktor. Musi być
 * wywołana przed pierwszym użyciem funkcji now[_ms|mu], ms, mu.
 */
Timer::Timer(){ init(); }