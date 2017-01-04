#include "Media.hpp"

class States : public Media{

private:

  int state = 0;

public:

  void set_state(int s){
    state = s;
    state_start = time_ms();
  }

  int get_state(){
    return state;
  }

};
