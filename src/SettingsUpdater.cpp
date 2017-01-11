#include "SettingsUpdater.hpp"

void update_settings(Database* db){
  lock_guard<mutex> lock(Task::settings_mutex);
  Task::settings.clear();
  auto res = db->query("select name, value from settings;");
  while(res->next())
    Task::settings[res->getString(1)] = res->getString(2);
}

SettingsUpdater::SettingsUpdater(Database* db){
  settings_thread = unique_ptr<thread>(new thread(update_settings, db));
}

SettingsUpdater::~SettingsUpdater(){
  if(settings_thread->joinable())
    settings_thread->join();
}
