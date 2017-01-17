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
#include "Common.hpp"
#include "Timer.hpp"
#include "Conditions.hpp"
#include "Scenario.hpp"
#include "Database.hpp"
#include "DataExchange.hpp"
#include "Gui.hpp"
#include "Media.hpp"
using namespace std;
// Musi być uwzględnione po .hpp z Task'a, inaczej problem z ms(dur d)
#include <SFML/Graphics.hpp>
using namespace sf;

#define STRINGIFY(s) PRE_STRINGIFY(s)
#define PRE_STRINGIFY(s) #s

// definiujemy te zmienne środowiskowe w zależności od wersji: 
// jeżeli release, to są inicjalizowane commitami obecnych stanów
// biblioteki i projektu, jeżeli nie release, to ="dev".
#ifdef LIB_SHA
string lib_sha = STRINGIFY(LIB_SHA);
#else
extern string lib_sha;
#endif
#ifdef PROJECT_SHA
string project_sha = STRINGIFY(PROJECT_SHA);
#else
extern string project_sha;
#endif

class Task : public Media {
private:

    time_type task_start;

protected:

    bool TRIAL_IS_OVER;

    vector<pair<string, vector<PType> > > design;
    int b, n;
    unsigned int nof_trials;
    unsigned int max_task_time;
    string table_name;
    static vector<string> saved_stages;
    bool initialized = false, finished = false;

    unique_ptr<Conditions> cs;
    unique_ptr<Scenario> scen;
    int current_trial;

    static int session_id;
    static map<string, PType> session_data;
    static bool user_data_initialized;
    map<string, PType> trial_data;

    void register_started_session();
    void register_started_task();
    void mark_task_finished();
    friend void update_session_status();

    void set_trial_data(string name, PType value) {
        if (trial_data.count(name) == 1)
            throw (runtime_error("set_trial_data: niedozwolona nazwa zmiennej: " + name));
        trial_data[name] = value;
    }

    void set_trial_data(initializer_list<pair<string, PType> >values) {
        for (auto& v : values)set_trial_data(v.first, v.second);
    }

    PType cnd(string f) {
        return cs->get(f, scen->get(current_trial));
    }
    string session_cnd();

    bool quit_key_pressed() {
        return (keyp(KEYESCAPE) > task_start) && 
                (keyp(KEYLCONTROL) > task_start) &&
                (keyr(KEYESCAPE) < keyp(KEYESCAPE)) &&
                (keyr(KEYLCONTROL) < keyp(KEYLCONTROL));
    };
public:

    void get_unfinished_sessions(void);

    static Database db;

    inline void display();

    friend void set_project_name(string project);
    friend void get_user_data(string instruction);
    friend string random_condition(vector<string> conditions = {});

    PType get_session_data(string name);

    void init(string task_name, vector<pair<string, vector<PType> > > design, unsigned int b = 1, unsigned int n = 1,
            unsigned int nof_trials = 0, unsigned int max_task_time = 0);
    void run(string task_name, vector<pair<string, vector<PType> > > design, unsigned int b = 1, unsigned int n = 1,
            unsigned int nof_trials = 0, unsigned int max_task_time = 0);
    void run();
    virtual void trial_code(int state) = 0;
    inline bool task_is_finished();

    bool measure_key_reaction(const vector<int>& response_keys, int& response, int& rt, const time_type& start);

    bool break_is_forced();
    void forced_break();
};

void Task::display() {
    if (debug) {
        string condition;
        for (auto& f : cs->factors)
            condition += f.first + ": " + to_string(cnd(f.first));
        Text text;
        text.setFont(font);
        text.setCharacterSize((float) height * 0.02);
        text.setString("state: " + to_string(state()) + " condition: " + condition);
        text.setFillColor(Color::Green);
        text.setPosition(Vector2f(0, 0));
        draw(text);
    }
    Media::display();
}

extern string user_data_instr;
extern string session_over_instr;

vector<int> vseq(int from, int to);
int random_int(int min, int max);
void set_project_name(string project);
void get_user_data(string fname);
string random_condition(vector<string> conditions);
void update_session_status();

#endif
