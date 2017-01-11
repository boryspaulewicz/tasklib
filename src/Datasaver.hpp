#ifndef DATASAVER
#define DATASAVER

#include "Common.hpp"
#include <map>
#include <set>
#include <string>
#include <thread>
#include "Database.hpp"
using namespace std;

class Datasaver{

private:

  unique_ptr<thread> send_data_thread;

public:

  Datasaver(Database* db, string& task_name, int& session_id, map<string, Ptype>& session_data, map<string, Ptype>& trial_data);

  ~Datasaver();
  
};

// void set_settings(string name, string value);
// void update_settings();

#endif
