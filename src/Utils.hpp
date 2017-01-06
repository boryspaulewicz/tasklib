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

vector<int> vseq(int from, int to);

#endif
