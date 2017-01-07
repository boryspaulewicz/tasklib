// -*- coding: utf-8 -*-

// pakiet libmysqlcppconn-dev

#ifndef DATABASE
#define DATABASE

#include "Common.hpp"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <mutex>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "Ptype.hpp"

using namespace std;
using namespace sql;

class Database{
private:

  mutex db_mutex;

  // tylko pobieramy wskaźnik do istniejącego wcześniej drivera
  Driver *driver;
  unique_ptr<Connection> con;
  unique_ptr<Statement> stmt;

  void exception(SQLException &e);

public:

  string password;
  
  void connect();
  void disconnect();
  unique_ptr<ResultSet> query(string q);
  void execute(string q);
  void register_session();
  string insert_statement(string table, map<string, Ptype>& d);
  bool table_exists(string table);
  
  ~Database();
};

#endif
