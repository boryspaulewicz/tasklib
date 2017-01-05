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

