#ifndef DATASAVER
#define DATASAVER

#include "Common.hpp"
#include <map>
#include <set>
#include <string>
#include <thread>
#include "Database.hpp"
using namespace std;

class DataExchange{

private:

  unique_ptr<thread> send_data_thread;
  unique_ptr<thread> settings_thread;
  static mutex settings_mutex;
  static map<string, string> settings;

public:

  DataExchange(Database* db, string& task_name, int& session_id, map<string, PType>& session_data, map<string, PType>& trial_data);
  ~DataExchange();
  
  friend string get_settings(string name);
  friend void send_data(Database* db, string table_name, int session_id, map<string, PType> session_data, map<string, PType> trial_data);
  friend void update_settings(Database* db);
};

string get_settings(string name);
void update_settings(Database* db);

#endif
