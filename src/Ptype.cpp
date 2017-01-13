#include "PType.hpp"

string to_string(const PType& v){
  switch(v.type){
  case PType::STRING:
    return v.sval;
  case PType::DOUBLE:
    return to_string(v.dval);
  case PType::INT:
    return to_string(v.ival);
  }
}

ostream& operator<<(ostream& os, const PType& v){ os << to_string(v); return os; }

istream& operator>>(istream& is, PType& v){
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

string operator+(string lhs, PType& rhs){ return lhs + (string)rhs; }

string operator+(const char* lhs, PType& rhs){ return string(lhs) + (string)rhs; }

string operator+(PType& lhs, const char* rhs){ return (string)lhs + string(rhs); }

map<string, PType> read_cfg(string fname, bool must_exist){
  map<string, PType> res;
  ifstream f;
  f.open(fname);
  string var;
  if(f.good()){
    while(f >> var){
      f >> res[var];
    }
  }else{
    auto msg = "Nie znalaz³em pliku konfiguracyjnego " + fname;
    if(must_exist){
      throw(runtime_error(msg));
    }else{
      log(msg);
    }
  }
  return res;
}
