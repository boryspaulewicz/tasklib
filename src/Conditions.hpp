// -*- coding: utf-8 -*-

#ifndef CONDITIONS
#define CONDITIONS

#include<vector>
#include<map>
#include<string>
#include<iostream>
using namespace std;

class Conditions{

public:

  map<string,vector<string> > factors;
  map<string,vector<string> > conditions;
  
  void fill_conditions();
  
  int nof_cnds;
  vector<string> names;
  
  Conditions(initializer_list<pair<string, vector<string> > > design);
  inline string get(string f, int c){
    return conditions[f].at(c);
  }
  void print();
};

#endif
