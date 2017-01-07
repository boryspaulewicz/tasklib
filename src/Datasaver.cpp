#include "Datasaver.hpp"

void send_data(Database* db, string task_name, int session_id, map<string, Ptype> session_data, map<string, Ptype> trial_data){
  string table_name = task_name + "_data";
  if(trial_data["trial"] == 0){
    if(db->table_exists(table_name)){
      auto desc = db->query("DESCRIBE " + table_name + ";");
      set<string> cols;
      while(desc->next())
        cols.insert(desc->getString(1));
      for(auto& x : trial_data)
        if(cols.find(x.first) == cols.end())
          db->execute("ALTER TABLE ADD " + x.first + " " + x.second.sql_type() + ";");
    }else{
      string q("CREATE TABLE " + table_name + "(timestamp TIMESTAMP, session_id INT(11) NOT NULL, ");
      for(auto& x : trial_data)
        q += x.first + " " + x.second.sql_type() + ",";
      q += "KEY session_id (session_id), FOREIGN KEY (session_id) REFERENCES session (session_id)"
        " ON DELETE CASCADE ON UPDATE CASCADE);";
      db->execute(q);
    }
  }
  trial_data["session_id"] = session_id;
  db->execute(db->insert_statement(task_name + "_data", trial_data));
}

Datasaver::Datasaver(Database* db, string& task_name, int& session_id, map<string, Ptype>& session_data,
                     map<string, Ptype>& trial_data){
  send_data_thread = new thread(send_data, db, task_name, session_id, session_data, trial_data);
}

Datasaver::~Datasaver(){
  send_data_thread->join();
}

