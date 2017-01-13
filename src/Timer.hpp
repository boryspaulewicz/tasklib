#ifndef TIMER
#define TIMER

// Funkcje mierzące czas mają, albo nie mają sufixu _ms/_mu. Jeżeli nie mają to 
// zwracają punkt czasowy, a jeżeli mają, to zwracają długość odcinka w odp.
// jednostkach (rezultat time_type);

#include"Common.hpp"
#include<chrono>

using namespace std;
using namespace std::chrono;

using tp = high_resolution_clock::time_point;
using dur = decltype(high_resolution_clock::now() - high_resolution_clock::now());
using time_type = long int;
inline tp now();

class Timer{
private:
    static tp start;
    static bool initialized;
    friend tp now();
    friend time_type now_ms();
    friend time_type now_mu();
public:
    Timer();
};

inline tp now(){ return high_resolution_clock::now(); }
inline time_type ms(dur d){ return duration_cast<milliseconds>(d).count(); }
inline time_type mu(dur d){ return duration_cast<microseconds>(d).count(); }
// Zakładamy, że Timer jest zaincjalizowany, ale nie sprawdzamy tego
inline time_type now_ms(){ return ms(now() - Timer::start); }
inline time_type now_mu(){ return mu(now() - Timer::start); }

#endif
