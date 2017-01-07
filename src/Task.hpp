// -*- coding: utf-8 -*-

// Oddzielamy inicjalizację od konstruktora, żeby ten sam obiekt mógł
// realizować różne wersje zadania.

#ifndef TASK
#define TASK

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
#include<stdexcept>
#include<memory>
#include<numeric>
#include<thread>
#include<chrono>
#include<functional>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Common.hpp"
#include "Conditions.hpp"
#include "Scenario.hpp"
#include "Database.hpp"
#include "Datasaver.hpp"
#include "Gui.hpp"
#include "Media.hpp"
#include "Timer.hpp"

class Task : public Media{

private:

  time_type task_start;
  
protected:

  bool TRIAL_IS_OVER;
  
  vector<pair<string, vector<Ptype> > > design;
  int b, n;
  unsigned int nof_trials;
  unsigned int max_task_time;
  string task_name;
  bool initialized = false, finished = false;
  
  unique_ptr<Conditions> cs;
  unique_ptr<Scenario> scen;
  int current_trial;

  static int session_id;
  static map<string, Ptype> session_data;
  static bool user_data_initialized;
  static bool sha_data_initialized;
  map<string, Ptype> trial_data;

  void register_session();
  void mark_session_finished();

  void set_trial_data(string name, Ptype value){ trial_data[name] = value; }
  void set_trial_data(initializer_list<pair<string, Ptype> >values){ for(auto& v : values)trial_data[v.first] = v.second; }
  
  Ptype cnd(string f){ return cs->get(f, scen->get(current_trial)); }

 public:

  static Database db;

  inline void display();

  friend void get_sha_data();
  friend void set_project_name(string project);
  friend void get_user_data(string instruction = "");
  friend string get_random_condition(string task_name, vector<string> conditions = {});

  Ptype get_session_data(string name);
  
  void init(string task_name, vector<pair<string, vector<Ptype> > > design, unsigned int b = 1, unsigned int n = 1,
            unsigned int nof_trials = 0, unsigned int max_task_time = 0);
  
  void run(string task_name, vector<pair<string, vector<Ptype> > > design, unsigned int b = 1, unsigned int n = 1,
           unsigned int nof_trials = 0, unsigned int max_task_time = 0);

  void run();

  virtual void trial_code(int state) = 0;

  bool measure_key_reaction(const vector<int>& response_keys, int& response, int& rt, const time_type& start);
  
  inline bool task_is_finished();

};

void Task::display(){
  if(debug){
    string condition;
    for(auto& f : cs->factors)
      condition += f.first + ": " + to_string(cnd(f.first));
    Text text;
    text.setFont(font); text.setCharacterSize((float)height * 0.02);
    text.setString("state: " + to_string(state()) + " condition: " + condition);
    text.setColor(Color::Green);
    text.setPosition(Vector2f(0, 0));
    draw(text);
  }
  Media::display();
}

extern string user_data_instr;
extern string session_over_instr;

vector<int> vseq(int from, int to);
int random_int(int min, int max);
void get_sha_data();
void set_project_name(string project);
void get_user_data(string fname);
string get_random_condition(string task_name, vector<string> conditions);

#endif
