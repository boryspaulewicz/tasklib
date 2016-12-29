// -*- coding: utf-8 -*-

// Oddzielamy inicjalizację od konstruktora, żeby ten sam obiekt mógł
// realizować różne wersje zadania.

#ifndef TASK
#define TASK

#include<random>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<stdexcept>
#include<memory>
#include<numeric>
#include<thread>
#include<chrono>
#include<functional>
using namespace std;
using namespace std::chrono;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Conditions.hpp"
#include "Scenario.hpp"
#include "Database.hpp"
#include "Gui.hpp"
#include "Media.hpp"

class Task :public Media{

protected:

  unique_ptr<Conditions> cs;
  unique_ptr<Scenario> scen;
  unsigned int nof_trials;
  unsigned int current_trial;

  map<string, string> trial_data;

  unique_ptr<thread>send_data_thread;
  
  static map<string, string> session_data;
  static int session_id;
  static bool user_data_initialized;
  static bool sha_data_initialized;

  void register_session();

  void mark_session_finished();

  template<typename T>
  void data(string name, T value){
    trial_data[name] = to_string(value);
  }

  static void send_data(string task_name, map<string, string> d);
  
  inline string cnd(string f){ return cs->get(f, scen->get(current_trial)); }

 public:

  bool use_db = true;
  static Database db;

  friend void get_sha_data();
  
  friend void set_project_name(string project);
  
  friend void get_user_data(string fname = "./userdata.utxt");

  friend string get_random_condition(string task_name, vector<string> conditions = {});

  void run(string task_name, initializer_list<pair<string, vector<string> > > levels = {{"f", {"A", "B"}}, {"g", {"1", "2", "3"}}},
           unsigned int b = 1, unsigned int n = 1, unsigned int nof_trials_ = 0);

  virtual bool trial_code(int state);
};

int random_int(int min, int max);

void get_sha_data();

void set_project_name(string project);

void get_user_data(string fname);

string get_random_condition(string task_name, vector<string> conditions);

#endif
