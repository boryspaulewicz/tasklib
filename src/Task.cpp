// -*- coding: utf-8 -*-

#include "Task.hpp"

// Elementy wspólne dla wszystkich sesji
Database Task::db;
map<string, string> Task::session_data;
int Task::session_id;
bool Task::user_data_initialized = false;
bool Task::sha_data_initialized = false;

#define LIB_SHA "lib_sha"
#define PROJECT_SHA "project_sha"

int random_int(int min, int max){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void get_sha_data(){
  if(Task::sha_data_initialized)
    return;
  string value;
  ifstream f;
  f.open(LIB_SHA);
  if(f.good()){
    getline(f, value);
    if(value.size() != 40)
      throw(runtime_error("Plik " LIB_SHA " ma niewłaściwą długość"));
    Task::session_data["lib_sha"] = "'" + value + "'";
  }else{
    cout << "Nie znalazłem pliku " LIB_SHA "." << endl;
  }
  f.close();
  f.open(PROJECT_SHA);
  if(f.good()){
    getline(f, value);
    if(value.size() != 40)
      throw(runtime_error("Plik " PROJECT_SHA " ma niewłaściwą długość"));
    Task::session_data["project_sha"] = "'" + value + "'";
  }else{
    cout << "Nie znalazłem pliku" PROJECT_SHA "." << endl;
  }
  Task::sha_data_initialized = true;
}

void set_project_name(string project){
  Task::session_data["project"] = "'" + project + "'";
}

void get_user_data(string fname){
  if(!Task::user_data_initialized){
    cout << "Pobieram dane osobowe" << endl;
    Instruction in(fname);
    Userdata ud;
    for(auto d : ud.data)
      Task::session_data[d.first] = d.second;
    Task::user_data_initialized = true;
  }
}

string get_random_condition(string task_name, vector<string> conditions){
  // liczebności również warunków, które jeszcze nie wystąpiły w bazie
  // (wtedy = 0)
  map<string, int> counts;
  for(auto c : conditions)
    counts[c] = 0;
  auto res = Task::db.query("select cnd, count(*) from session where task = '" + task_name +
                            "' and stage = 'finished' and name != 'admin' group by cnd;");
  while(res->next()){
    counts[res->getString(1)] = res->getInt(2);
    conditions.push_back(res->getString(1));
  }

  cout << "Liczba sesji na warunek " + task_name + ":" << endl;
  for(auto c : counts)
    cout << c.first << ": " << c.second << endl;

  string chosen;
  if(Task::session_data["name"] == "admin"){
    Chooseitem ci(conditions, "Wybór administratora");
    chosen = ci.value;
  }else{
    // Zwracamy pierwszy z najmniej reprezentowanych
    int min_count = counts[conditions[0]];
    int j = 0;
    for(int i = 1; i < counts.size(); i++){
      int count = counts[conditions[i]];
      if(count < min_count){
        min_count = count;
        j = i;
      }
    }
    chosen = conditions[j];
  }
  Task::session_data["cnd"] = "'" + chosen + "'";
  cout << "Wybrany warunek: " + chosen << endl;
  return chosen;
}

void Task::register_session(){
  if(user_data_initialized){
    cout << "Rejestruję sesję" << endl;
    db.execute(db.insert_statement("session", session_data));
    auto r = db.query("SELECT LAST_INSERT_ID();");
    if(r->next())
      session_id = r->getInt(1);
    cout << "session_id: " << session_id << endl;
  }else{
    throw(runtime_error("Brak unikalnego identyfikatora, nie mogę zarejestrować tej sesji."));
  }
}

void Task::send_data(string task_name, map<string, string> d){
  if(d["trial"] == "0"){
    string q("CREATE TABLE IF NOT EXISTS " + task_name + "_data "
             "(timestamp TIMESTAMP, session_id INT(11) NOT NULL, ");
    for(auto& x : d){
      q += x.first + " " + (x.second[0] == '\'' ? "VARCHAR(50), " : "DOUBLE, ");
    }
    q += "KEY session_id (session_id), FOREIGN KEY (session_id) REFERENCES session (session_id)"
      " ON DELETE CASCADE ON UPDATE CASCADE);";
    db.execute(q);
  }
  d["session_id"] = to_string(session_id);
  db.execute(db.insert_statement(task_name + "_data", d));
}

void Task::run(string task_name, initializer_list<pair<string, vector<string> > > levels, unsigned int b, unsigned int n, unsigned int nof_trials_){

  if(getenv("TASKLIB_NODB") != nullptr)
    use_db = false;

  if(task_name == "")
    throw(runtime_error("Nie podano nazwy zadania"));
  session_data["task"] = "'" + task_name + "'";

  get_user_data();
  get_sha_data();
  if(use_db){
    db.connect();
    register_session();
  }
    
  cs = unique_ptr<Conditions>(new Conditions(levels));
  srand(time(NULL));
  scen = unique_ptr<Scenario>(new Scenario(cs->nof_cnds, b, n));
  if(nof_trials_ == 0){
    nof_trials = scen->size();
  }else if(nof_trials_ > scen->size()){
    throw(std::runtime_error("Liczba prób większa niż długość scenariusza"));
  }else{
    nof_trials = nof_trials_;
  }
  cout << "Długość zadania: " << nof_trials  << endl;
  cs->print();
    
  Media::init();

  cout << "Rozpoczynam pętlę prób zadania" << endl;
  task_start = high_resolution_clock::now();
  for(current_trial = 0; current_trial < nof_trials; current_trial++){

    trial_data.clear();
    trial_data["trial"] = to_string(current_trial);
    for(auto& f : cs->names)
      trial_data[f] = cnd(f);
    state = 0;
    state_start = trial_start = high_resolution_clock::now();

    while(trial_code(state) && window->isOpen())
      process_events(event);
    if(!window->isOpen())
      break;
    
    if(use_db){
      if(send_data_thread != nullptr)
        send_data_thread->join();
      send_data_thread = unique_ptr<thread>(new thread(send_data, task_name, trial_data));
    }
  }
    
  if(use_db){
    send_data_thread->join();
    if(current_trial == nof_trials)
      mark_session_finished();
    db.disconnect();
  }

  Media::close();
}

void Task::mark_session_finished(){
  db.execute("UPDATE session SET stage = \"finished\" WHERE session_id = " + to_string(session_id) + ";");
}
