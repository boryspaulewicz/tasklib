// -*- coding: utf-8 -*-

#include "Database.hpp"
#include "Gui.hpp"

#define DATABASE_NAME "task2"

void Database::exception(SQLException &e){
  log("# ERR: SQLException in " +
      string(__FILE__) + "(" + string(__FUNCTION__) + ") on line " +
      to_string(__LINE__) + "\n" + "# ERR: " + string(e.what()) +
      " (MySQL error code: " + to_string(e.getErrorCode()) +
      ", SQLState: " + string(e.getSQLState()) + " )");
  exit(1);
}

bool Database::connection_is_closed(){
  if(con != nullptr){
    lock_guard<mutex> lock(db_mutex);
    return con->isClosed();
  }else{
    return true;
  }
}

unique_ptr<ResultSet> Database::query(string q){
  try{
    if(connection_is_closed())
      connect();
    log("query: " + q);
    { lock_guard<mutex> lock(db_mutex);
      return unique_ptr<ResultSet>(stmt->executeQuery(q)); }
  }catch(SQLException &e){
    exception(e);
  }
}

void Database::execute(string q){
  try{
    if(connection_is_closed())
      connect();
    log("execute: " + q);
    { lock_guard<mutex> lock(db_mutex);
      stmt->execute(q); }
  }catch(SQLException &e){
    exception(e);
  }
}

string Database::match_statement(map<string, Ptype>& d){
  string res;
  auto it = d.begin();
  while(it != d.end()){
    res += it->first + " = " + it->second.sql_val();
    if(++it != d.end())
      res += " AND ";
  }
  return res;
}

string Database::insert_statement(string table, map<string, Ptype> d){
  stringstream cols, vals;
  for(auto x = d.begin(); x != d.end(); x++){
    cols << x->first;
    vals << x->second.sql_val();
    if(x != --d.end()){
      cols << ",";
      vals << ",";
    }
  }
  return "INSERT INTO " + table + " (" + cols.str() + ") VALUES (" + vals.str() + ");";
}

bool Database::table_exists(string table){
  execute("USE information_schema");
  auto res = query("SELECT * FROM TABLES WHERE TABLE_SCHEMA = '" DATABASE_NAME "' AND TABLE_NAME = '" + table + "';");
  execute("USE " DATABASE_NAME ";");
  return res->next();
}

void Database::connect(){
  if(!connection_is_closed())
    return;

  if(password == ""){
    if(getenv("TASKLIB") == nullptr){
      Uservalue uv({"Podaj hasło:"}, false);
      password = uv.value[0];
    }else{
      password = string(getenv("TASKLIB"));
    }
  }
  
  try{
    { lock_guard<mutex> lock(db_mutex);
      driver = get_driver_instance();
      con = unique_ptr<Connection>(driver->connect("tcp://5.189.166.138:443", "task", password));
      con->setSchema(DATABASE_NAME);
      stmt = unique_ptr<Statement>(con->createStatement()); }
    auto res = query("SELECT 'Ustanowiłem połączenie z bazą danych' AS message");
    while(res->next())
      log(res->getString("message"));
    execute("SET NAMES utf8;");
  }catch(SQLException &e){
    exception(e);
  }
}

void Database::disconnect(){
  if(!connection_is_closed()){
    log("Zamykam połączenie z bazą danych");
    lock_guard<mutex> lock(db_mutex);
    con->close();
    con = nullptr;
  }
}

Database::~Database(){
  disconnect();
}
