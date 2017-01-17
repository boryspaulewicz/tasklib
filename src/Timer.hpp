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
/**
 * Aktualny czas jako punkt czasowy (tp)
 * @return 
 */
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
    static void init();
};

inline tp now(){ return high_resolution_clock::now(); }
/**
 * Zamienia odcinek czasu na milisekundy
 * @param d odcinek czasu
 * @return 
 */
inline time_type ms(dur d){ return duration_cast<milliseconds>(d).count(); }
/**
 * Zamienia odcinek czasu na mikrosekundy
 * @param d odcinek czasu
 * @return 
 */
inline time_type mu(dur d){ return duration_cast<microseconds>(d).count(); }
// Zakładamy, że Timer jest zaincjalizowany, ale nie sprawdzamy tego
/**
 * Czas w milisekundach od utworzenia pierwszego obiektu (dziedziczącego z) Timer
 * @return 
 */
inline time_type now_ms(){ return ms(now() - Timer::start); }
/**
 * Czas w mikrosekundach od utworzenia pierwszego obiektu (dziedziczącego z) Timer
 * @return 
 */
inline time_type now_mu(){ return mu(now() - Timer::start); }

#endif
