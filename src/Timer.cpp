#include "Timer.hpp"

tp Timer::start;

Timer::Timer(){
  if(!start_set)
    start = high_resolution_clock::now();
  start_set = true;
}
