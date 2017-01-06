#include "Utils.hpp"

string trim(string str){
  size_t first = str.find_first_not_of(' ');
  if (first == string::npos)
    return "";
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last-first+1));
}

bool rmatch(string pattern, string str){
  smatch m;
  regex re(pattern);
  return regex_match(str, m, re);
}

int random_int(int min, int max){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

vector<int> vseq(int from, int to){
  vector<int> res;
  for(; from <= to; from++)
    res.push_back(from);
  return res;
}
