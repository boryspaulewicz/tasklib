#include "Ptype.hpp"

string to_string(const Ptype& v){
  switch(v.type){
  case Ptype::STRING:
    return v.sval;
  case Ptype::DOUBLE:
    return to_string(v.dval);
  case Ptype::INT:
    return to_string(v.ival);
  }
}

ostream& operator<<(ostream& os, const Ptype& v){ os << to_string(v); return os; }

istream& operator>>(istream& is, Ptype& v){
  string s;
  is >> s;
  if(rmatch("^[0-9]+$", s)){
    v = stoi(s);
  }else if(rmatch("^[0-9]+\\.[0-9]+$", s)){
    v = stod(s);
  }else{
    v = s;
  }
  return is;
}

map<string, Ptype> read_cfg(string fname){
  map<string, Ptype> res;
  ifstream f;
  f.open(fname);
  string var;
  if(f.good()){
    while(f >> var){
      f >> res[var];
    }
  }else{
    throw(runtime_error("Nie znalaz³em pliku " + fname));
  }
  return res;
}
