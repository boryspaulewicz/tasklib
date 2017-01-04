// -*- coding: utf-8 -*-

#include "Task.hpp"

// Elementy wspólne dla wszystkich zadań
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
  ifstream f;
  f.open(LIB_SHA);
  string value;
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

void get_user_data(string instr){
  if(!Task::user_data_initialized){
    cout << "Pobieram dane osobowe" << endl;
    Instruction in(instr);
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

string Task::get_session_data(string name){
  return session_data[name];
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
    throw(runtime_error("Brak danych osobowych, nie mogę zarejestrować tej sesji."));
  }
}

bool Task::task_is_finished(){
  if(!finished)
    finished = (current_trial == nof_trials) || ((max_task_time > 0) && (task_time() >= max_task_time));
  return finished;
}

void Task::mark_session_finished(){
  db.execute("UPDATE session SET stage = \"finished\" WHERE session_id = " + to_string(session_id) + ";");
}

void Task::init(string task_name_, vector<pair<string, vector<string> > > design_,
                unsigned int b_, unsigned int n_, unsigned int nof_trials_,
                unsigned int max_task_time_){
  task_name = task_name_; design = design_;
  b = b_; n = n_; nof_trials = nof_trials_;
  max_task_time = max_task_time_;
    
  if(getenv("TASKLIB_NODB") != nullptr)
    use_db = false;

  if(getenv("TASKLIB_DEBUG") != nullptr)
    debug = true;

  if(task_name == "")
    throw(runtime_error("Nie podano nazwy zadania"));
  session_data["task"] = "'" + task_name + "'";

  initialized = true;
  finished = false;
}

void Task::run(){
  if(!initialized)
    throw(runtime_error("Wywołano funkcję run, ale zadanie nie zostało zainicjalizowane"));

  cs = unique_ptr<Conditions>(new Conditions(design));
  srand(time(NULL));
  scen = unique_ptr<Scenario>(new Scenario(cs->nof_cnds, b, n));
  if(nof_trials == 0){
    nof_trials = scen->size();
  }else if(nof_trials > scen->size()){
    throw(std::runtime_error("Liczba prób większa niż długość scenariusza"));
  }

  cout << "Długość zadania: " << nof_trials  << endl;
  cs->print();

  get_sha_data();
  if(use_db){
    db.connect();
    register_session();
  }
    
  Media::init();

  cout << "Rozpoczynam pętlę prób zadania" << endl;
  task_start = time_ms();
  for(current_trial = 0; !task_is_finished(); current_trial++){

    trial_data.clear();
    trial_data["trial"] = to_string(current_trial);
    for(auto& f : cs->names)
      trial_data[f] = cnd(f);

    if(measure_state_durations)
      state_durations.clear();
    
    state = 0;
    trial_start = time_ms();
    state_start = trial_start;
    
    while((trial_code(state) == NOT_OVER) && (keyp(KEYESCAPE) <= task_start))
      process_events(event);
    
    if(keyp(KEYESCAPE) > task_start)
      break;

    if(measure_state_durations){
      cout << "Czas trwania stanów (mu) ";
      for(auto& d : state_durations)
        cout << to_string(d.first) << ": " << to_string(d.second) + " ";
      cout << endl;
    }

    if(use_db)
      unique_ptr<Datasaver> ds = unique_ptr<Datasaver>(new Datasaver(&db, task_name, session_id, session_data, trial_data));
  }

  if(use_db){
    if(task_is_finished())
      mark_session_finished();
    db.disconnect();
  }

  Media::close();
  
  cout << "Zadanie trwało " << floor(task_time() / 60000) << " minut." << endl;
}
