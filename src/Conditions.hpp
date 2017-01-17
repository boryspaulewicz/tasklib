// -*- coding: utf-8 -*-

#ifndef CONDITIONS
#define CONDITIONS

#include "Common.hpp"
#include<vector>
#include<map>
#include<string>
#include<iostream>
#include"PType.hpp"
using namespace std;

class Conditions {
public:

    map<string, vector<PType> > factors;
    map<string, vector<PType> > conditions;

    int nof_cnds;
    vector<string> names;

    void fill_conditions();

    Conditions(vector<pair<string, vector<PType> > > design);

    PType get(string f, int c) {
        return conditions[f].at(c);
    }

    void print();
};

#endif
