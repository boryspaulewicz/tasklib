// -*- coding: utf-8 -*-

#ifndef CONDITIONS
#define CONDITIONS

#include "Common.hpp"
#include<vector>
#include<map>
#include<string>
#include<iostream>
#include"Ptype.hpp"
using namespace std;

class Conditions{

public:

  map<string,vector<Ptype> > factors;
  map<string,vector<Ptype> > conditions;
  
  int nof_cnds;
  vector<string> names;

  void fill_conditions();
  
  Conditions(vector<pair<string, vector<Ptype> > > design);

  Ptype get(string f, int c){ return conditions[f].at(c); }

  void print();
};

#endif
