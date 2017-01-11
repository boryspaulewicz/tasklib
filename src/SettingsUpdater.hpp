#ifndef SETTINGS
#define SETTINGS

#include "Task.hpp"
#include <map>
#include <string>
#include <thread>

class SettingsUpdater{
private:

  unique_ptr<thread> settings_thread;

public:

  SettingsUpdater(Database* db);
  ~SettingsUpdater();

};

void update_settings(Database* db);

#endif
