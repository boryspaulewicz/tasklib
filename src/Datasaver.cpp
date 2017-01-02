#include "Datasaver.hpp"

void send_data(Database* db, string task_name, int session_id, map<string, string> session_data, map<string, string> trial_data){
  if(trial_data["trial"] == "0"){
    string q("CREATE TABLE IF NOT EXISTS " + task_name + "_data "
             "(timestamp TIMESTAMP, session_id INT(11) NOT NULL, ");
    for(auto& x : trial_data){
      q += x.first + " " + (x.second[0] == '\'' ? "VARCHAR(50), " : "DOUBLE, ");
    }
    q += "KEY session_id (session_id), FOREIGN KEY (session_id) REFERENCES session (session_id)"
      " ON DELETE CASCADE ON UPDATE CASCADE);";
    db->execute(q);
  }
  trial_data["session_id"] = to_string(session_id);
  db->execute(db->insert_statement(task_name + "_data", trial_data));
}

Datasaver::Datasaver(Database* db, string task_name, int session_id, map<string, string> session_data,
                     map<string, string> trial_data){
  send_data_thread = new thread(send_data, db, task_name, session_id, session_data, trial_data);
}

Datasaver::~Datasaver(){
  send_data_thread->join();
}

