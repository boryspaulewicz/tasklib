// -*- coding: utf-8 -*-

#ifndef SCENARIO
#define SCENARIO

#include<vector>
#include<stdlib.h>
using namespace std;

class Scenario{
private:

  // k: liczba warunków, b: wielkość bloku permutowanego jako
  // wielokrotność liczby warunków, n: liczba bloków
  unsigned int k, b, n;
  vector<unsigned int> scen;

public:

  Scenario(unsigned int k_, unsigned int b_, unsigned int n_);
  inline unsigned int get(unsigned int i){
    return scen.at(i);
  }
  unsigned int size();
  void permute();
};

#endif
