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

  enum TRIAL_STATUS{OVER, NOT_OVER};
  
  vector<pair<string, vector<string> > > design;
  int b, n;
  unsigned int nof_trials;
  string task_name;
  bool was_initialized = false;
  
  unique_ptr<Conditions> cs;
  unique_ptr<Scenario> scen;
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

  template<typename T>
  void data(vector<pair<string, T> > values){
    for(auto& v : values)
    trial_data[v.first] = to_string(v.second);
  }
  
  static void send_data(string task_name, map<string, string> d);
  
  inline string cnd(string f){ return cs->get(f, scen->get(current_trial)); }

 public:

  bool debug = false;

  inline void display();

  bool use_db = true;
  static Database db;

  friend void get_sha_data();
  
  friend void set_project_name(string project);

  friend void get_user_data(string instruction);

  friend string get_random_condition(string task_name, vector<string> conditions = {});

  void init(string task_name, vector<pair<string, vector<string> > > levels = {{"f", {"A", "B"}}, {"g", {"1", "2", "3"}}},
            unsigned int b = 1, unsigned int n = 1, unsigned int nof_trials_ = 0);
  
  void run();

  virtual TRIAL_STATUS trial_code(int state) = 0;
};

void Task::display(){
  if(debug){
    string condition;
    for(auto& f : cs->factors)
      condition += f.first + ": " + cnd(f.first);
    Text text;
    text.setFont(font); text.setCharacterSize((float)height * 0.02);
    text.setString("state: " + to_string(state) + " condition: " + condition);
    text.setPosition(Vector2f(0, 0));
    draw(text);
  }
  Media::display();
}

int random_int(int min, int max);

void get_sha_data();

void set_project_name(string project);

void get_user_data(string fname);

string get_random_condition(string task_name, vector<string> conditions);

#endif
