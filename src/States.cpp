#include "Timer.hpp"

class States : public Timer{
private:
  int state;
  time_type state_start;
  
public:
  void set_state(int s){
    state = s;
    state_start = time_ms();
  }

  time_type state_time(){ return time_ms() - state_start; }
};
