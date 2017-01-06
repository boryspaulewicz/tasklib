#ifndef UTILS
#define UTILS

#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <regex>
#include <map>
using namespace std;

string trim(string str);

bool rmatch(string pattern, string str);

int random_int(int min, int max);

template<typename T>
vector<T> vseq(T from, T to){
  vector<T> res;
  for(; from != to; from++)
    res.push_back(from);
  return res;
}

template<typename T>
void permute(vector<T>& scen){
  T stored;
  int rand_i;
  for(int i = 0; i < scen.size() - 1; i++){
    stored = scen[i];
    rand_i = rand() % scen.size();
    scen[i] = scen[rand_i];
    scen[rand_i] = stored;
  }
}

#endif
