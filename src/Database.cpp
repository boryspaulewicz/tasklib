// -*- coding: utf-8 -*-

#include "Database.hpp"
#include "Gui.hpp"

void Database::exception(SQLException &e){
  log("# ERR: SQLException in " +
      string(__FILE__) + "(" + string(__FUNCTION__) + ") on line " +
      to_string(__LINE__) + "\n" + "# ERR: " + string(e.what()) +
      " (MySQL error code: " + to_string(e.getErrorCode()) +
      ", SQLState: " + string(e.getSQLState()) + " )");
  exit(1);
}

unique_ptr<ResultSet> Database::query(string q){
  try{
    db_mutex.lock();
    if(con == nullptr || con->isClosed()){
      connect();
    }
    db_mutex.unlock();
    log("query: " + q);
    return unique_ptr<ResultSet>(stmt->executeQuery(q));
  }catch(SQLException &e){
    exception(e);
  }
}

void Database::execute(string q){
  try{
    db_mutex.lock();
    if(con == nullptr || con->isClosed()){
      connect();
    }
    log("execute: " + q);
    stmt->execute(q);
    db_mutex.unlock();
  }catch(SQLException &e){
    exception(e);
  }
}

string Database::insert_statement(string table, map<string, Ptype>& d){
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
  auto res = query("SELECT * FROM TABLES WHERE TABLE_SCHEMA = 'task' AND TABLE_NAME = '" + table + "';");
  execute("USE task;");
  return res->next();
}

void Database::connect(){
  { lock_guard<mutex> lock(db_mutex);
    if(!(con == nullptr || con->isClosed()))
      return;
  }
    
  if(password == ""){
    if(getenv("TASKLIB") == nullptr){
      Uservalue uv({"Podaj hasło:"}, false);
      password = uv.value[0];
    }else{
      password = string(getenv("TASKLIB"));
    }
  }
  
  try{
    db_mutex.lock();
    driver = get_driver_instance();
    con = unique_ptr<Connection>(driver->connect("tcp://5.189.166.138:443", "task", password));
    con->setSchema("task");
    stmt = unique_ptr<Statement>(con->createStatement());
    db_mutex.unlock();
    unique_ptr<ResultSet> res(query("SELECT 'Ustanowiłem połączenie z bazą danych' AS message"));
    db_mutex.lock();
    while(res->next()){
      log(res->getString("message"));
    }
    db_mutex.unlock();
    execute("SET NAMES utf8;");
  }catch(SQLException &e){
    exception(e);
  }
}

void Database::disconnect(){
  db_mutex.lock();
  if(!(con == nullptr || con->isClosed())){
    if(con != nullptr)
      log("Zamykam połączenie z bazą danych");
    con->close();
    con = nullptr;
  }
  db_mutex.unlock();
}

Database::~Database(){
  disconnect();
}
