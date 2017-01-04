#ifndef TIMER
#define TIMER

#include<chrono>
using namespace std::chrono;
using tp = high_resolution_clock::time_point;
using time_type = long int;

class Timer{

private:

  static tp start;
  bool start_set = false;
  
public:

  Timer();

  inline time_type time_ms(){
    return duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() -
                                                    start).count();
  }

  inline time_type time_mu(){
    return duration_cast<std::chrono::microseconds>(high_resolution_clock::now() -
                                                    start).count();
  }

};

#endif
