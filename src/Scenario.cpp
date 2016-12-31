// -*- coding: utf-8 -*-

#include "Scenario.hpp"

unsigned int Scenario::size(){
  return scen.size();
}

void Scenario::permute(){
  int from, to, stored, rand_i;
  for(int i = 0; i < n; i++){
    // Poczatek i koniec permutacji do przetasowania
    from = i*k*b;
    to = (i+1)*k*b;
    // Wypelniamy permutację kolejnymi wartosciami
    for(int i = from; i < to; i++){
      scen[i] = (i - from) % k;
    }
    // Zastępujemy elementy od pierwszego do n-1 wylosowanymi
    // (algorytm permutacji Knutha)
    for(int i = from; i < (to-1); i++){
      stored = scen[i];
      rand_i = rand() % (k * b);
      scen[i] = scen[from + rand_i];
      scen[from + rand_i] = stored;
    }
  }
}

Scenario::Scenario(unsigned int k_, unsigned int b_, unsigned int n_) : k{k_}, b{b_}, n{n_}{
  scen.resize(k * b * n);
  permute();
}
