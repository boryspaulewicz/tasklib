#ifndef PTYPE
#define PTYPE

#include "Common.hpp"
#include <string>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class PType{

public:

  enum val_type{STRING, DOUBLE, INT};

  string sval;
  double dval;
  int ival;

  val_type type;
  int varchar_size = 50;

  PType() : type(INT), ival{0}{}
  
  string sql_type(){
    switch(type){
    case STRING:
      return "VARCHAR(" + to_string(varchar_size) + ")";
    case INT:
      return "INT";
    case DOUBLE:
      return "DOUBLE";
    }
  }

  string sql_val(){
    switch(type){
    case STRING:
      return "'" + (string)*this + "'";
    default:
      return (string)*this;
    }
  }
  
  template<size_t N>
  PType(const char (&v)[N]){ type = STRING; sval = (const char*)v; }
  PType(const string& v){ type = STRING; sval = v; }
  PType(const double& v){ type = DOUBLE; dval = v; }
  PType(const int& v){ type = INT; ival = v; }

  template<size_t N>
  void operator=(const char (&v)[N]){ type = STRING; sval = string((const char*)v); }
  void operator=(const string& v){ type = STRING; sval = v; }
  void operator=(const double& v){ type = DOUBLE; dval = v; }
  void operator=(const int& v){ type = INT; ival = v; }

  template<class T>
  bool operator==(const T& v){ return (T)(*this) == v; }
  template<size_t N>
  bool operator==(const char (&v)[N]){ return (string)(*this) == v; }
  template<class T>
  bool operator!=(const T& v){ return (T)(*this) != v; }
  template<size_t N>
  bool operator!=(const char (&v)[N]){ return (string)(*this) != v; }
  
  operator double(){
    switch(type){
    case STRING:
      return stod(sval);
    case DOUBLE:
      return dval;
    case INT:
      return (double)ival;
    }
  }

  operator int(){
    switch(type){
    case STRING:
      return stoi(sval);
    case DOUBLE:
      return (int)dval;
    case INT:
      return ival;
    }
  }

  operator string(){
    switch(type){
    case PType::STRING:
      return sval;
    case PType::DOUBLE:
      return to_string(dval);
    case PType::INT:
      return to_string(ival);
    }
  }

};

string to_string(const PType& v);

ostream& operator<<(ostream& os, const PType& v);
istream& operator>>(istream& os, PType& v);

string operator+(string lhs, PType& rhs);
string operator+(const char* lhs, PType& rhs);
string operator+(PType& lhs, const char* rhs);

map<string, PType> read_cfg(string fname, bool must_exist = true);

#endif
