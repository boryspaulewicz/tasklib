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

string operator+(string lhs, Ptype& rhs){ return lhs + (string)rhs; }

string operator+(const char* lhs, Ptype& rhs){ return string(lhs) + (string)rhs; }

string operator+(Ptype& lhs, const char* rhs){ return (string)lhs + string(rhs); }

map<string, Ptype> read_cfg(string fname, bool must_exist){
  map<string, Ptype> res;
  ifstream f;
  f.open(fname);
  string var;
  if(f.good()){
    while(f >> var){
      f >> res[var];
    }
  }else{
    auto msg = "Nie znalaz�em pliku konfiguracyjnego " + fname;
    if(must_exist){
      throw(runtime_error(msg));
    }else{
      log(msg);
    }
  }
  return res;
}
