#ifndef DATASAVER
#define DATASAVER

#include <map>
#include <string>
#include <thread>
#include "Database.hpp"
using namespace std;

class Datasaver{

private:

  thread* send_data_thread = nullptr;

public:

  Datasaver(Database* db, string& task_name, int& session_id, map<string, Ptype>& session_data, map<string, Ptype>& trial_data);

  ~Datasaver();
  
};

#endif
