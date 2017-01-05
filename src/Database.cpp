// -*- coding: utf-8 -*-

#include "Database.hpp"
#include "Gui.hpp"

void Database::exception(SQLException &e){
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line "
       << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  exit(1);
}

unique_ptr<ResultSet> Database::query(string q){
  try{
    db_mutex.lock();
    if(con == nullptr || con->isClosed()){
      connect();
    }
    db_mutex.unlock();
    cout << "query: " << q << endl;
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
    cout << "execute: " << q << endl;
    stmt->execute(q);
    db_mutex.unlock();
  }catch(SQLException &e){
    exception(e);
  }
}

string Database::insert_statement(string table, map<string, string> &d){
  stringstream cols, vals;
  for(auto x = d.begin(); x != d.end(); x++){
    cols << x->first;
    vals << x->second;
    if(x != --d.end()){
      cols << ",";
      vals << ",";
    }
  }
  return "INSERT INTO " + table + " (" + cols.str() + ") VALUES (" + vals.str() + ");";
}

void Database::connect(){
  { lock_guard<mutex> lock(db_mutex);
    if(!(con == nullptr || con->isClosed()))
      return;
  }
    
  if(password == ""){
    if(getenv("TASKLIB") == nullptr){
      Uservalue uv("Podaj hasło:", false);
      password = uv.value;
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
      cout << res->getString("message") << endl;
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
    con->close();
    con = nullptr;
    cout << "Zamykam ewentualne połączenie z bazą danych" << endl;
  }
  db_mutex.unlock();
}

Database::~Database(){
  disconnect();
}
