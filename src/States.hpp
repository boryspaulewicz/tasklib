#ifndef STATES
#define STATES

#include "Timer.hpp"

class States : public Timer{

private:

  time_type state_start_ms;
  int current_state = 0;
  
public:

  void set_state(int s){
    current_state = s;
    state_start_ms = time_ms();
  }

  time_type state_time(){ return time_ms() - state_start_ms; }
  time_type state_start(){ return state_start_ms; }
  
  int state(){ return current_state; }

};

#endif
