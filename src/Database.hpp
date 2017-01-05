// -*- coding: utf-8 -*-

// pakiet libmysqlcppconn-dev

#ifndef DATABASE
#define DATABASE

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

  static mutex db_mutex;

  Driver *driver; // tylko pobieramy wskaźnik do istniejącego wcześniej drivera
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
  string insert_statement(string table, map<string, string> &d);

  ~Database();
};

#endif
