#include "DataExchange.hpp"

mutex DataExchange::settings_mutex;
map<string, string> DataExchange::settings;

string get_settings(string name){
  lock_guard<mutex> lock(DataExchange::settings_mutex);
  return DataExchange::settings.count(name) == 1 ? DataExchange::settings[name] : "";
}

void update_settings(Database* db){
  log("Uaktualniam ustawienia z bazy");
  lock_guard<mutex> lock(DataExchange::settings_mutex);
  DataExchange::settings.clear();
  auto res = db->query("select name, value from settings;");
  while(res->next())
    DataExchange::settings[res->getString(1)] = res->getString(2);
}

void send_data(Database* db, string table_name, int session_id, map<string, PType> session_data, map<string, PType> trial_data){
  if(trial_data["trial"] == 0){
    log("Zapisuj� dane do bazy");
    if(db->table_exists(table_name)){
      auto desc = db->query("DESCRIBE " + table_name + ";");
      set<string> cols;
      while(desc->next())
        cols.insert(desc->getString(1));
      for(auto& x : trial_data)
        if(cols.find(x.first) == cols.end())
          db->execute("ALTER TABLE " + table_name + " ADD " + x.first + " " + x.second.sql_type() + ";");
    }else{
      string q("CREATE TABLE " + table_name + "(session_id INT(11) NOT NULL, timestamp TIMESTAMP, ");
      for(auto& x : trial_data)
        q += x.first + " " + x.second.sql_type() + ",";
      q += "KEY (session_id), FOREIGN KEY (session_id) REFERENCES session (session_id) ON DELETE CASCADE);";
      db->execute(q);
    }
  }
  trial_data["session_id"] = session_id;
  db->execute(db->insert_statement(table_name, trial_data));
  update_settings(db);
}

DataExchange::DataExchange(Database* db, string& table_name, int& session_id, map<string, PType>& session_data,
                     map<string, PType>& trial_data){
  send_data_thread = unique_ptr<thread>(new thread(send_data, db, table_name, session_id, session_data, trial_data));
}

DataExchange::~DataExchange(){
  send_data_thread->join();
}

