#ifndef PTYPE
#define PTYPE

#include <string>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Ptype{

public:

  enum val_type{STRING, DOUBLE, INT};

  string sval;
  double dval;
  int ival;

  val_type type;
  int varchar_size = 50;

  Ptype() : type(INT), ival{0}{}
  
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
  Ptype(const char (&v)[N]){ type = STRING; sval = (const char*)v; }
  Ptype(const string& v){ type = STRING; sval = v; }
  Ptype(const double& v){ type = DOUBLE; dval = v; }
  Ptype(const int& v){ type = INT; ival = v; }

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
    case Ptype::STRING:
      return sval;
    case Ptype::DOUBLE:
      return to_string(dval);
    case Ptype::INT:
      return to_string(ival);
    }
  }

};

string to_string(const Ptype& v);

ostream& operator<<(ostream& os, const Ptype& v);

#endif
