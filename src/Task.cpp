// -*- coding: utf-8 -*-

#include "Task.hpp"

// Elementy wspólne dla wszystkich zadań
Database Task::db;
map<string, Ptype> Task::session_data;
int Task::session_id;
bool Task::user_data_initialized = false;
bool Task::sha_data_initialized = false;

#define LIB_SHA "lib_sha"
#define PROJECT_SHA "project_sha"

string user_data_instr = "W czasie eksperymentu obowiązuje cisza. Wyłącz telefon komórkowy. "
  "W razie jakichkolwiek wątpliwości nie wołaj osoby prowadzącej, tylko podnieś do góry rękę. "
  "Osoba prowadząca podejdzie w dogodnym momencie i postara się udzielić wszelkich wyjaśnień. "
  "Badanie jest anonimowe.\n\n"
  "Za chwilę zostaniesz poproszona/y o podanie danych: wieku, płci oraz identyfikatora. "
  "Identyfikator składa się z inicjałów (małymi literami) oraz czterech cyfr: dnia i miesiąca urodzenia (np. ms0706). "
  "Zwróć uwagę, że identyfikator składa się dokładnie z 6 znaków, a między inicjałami i dniem i miesiącem urodzenia nie ma spacji.";

string session_over_instr = "To już koniec eksperymentu. Dziękujemy za udział.\n\n"
  "Poczekaj na swoim miejscu i zachowaj ciszę, osoba prowadząca badanie podejdzie do Ciebie"
  " w dogodnym momencie i poinformuje Cię o dalszym postępowaniu.";

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
    Task::session_data["lib_sha"] = value;
  }else{
    log("Nie znalazłem pliku " LIB_SHA ".");
  }
  f.close();
  f.open(PROJECT_SHA);
  if(f.good()){
    getline(f, value);
    if(value.size() != 40)
      throw(runtime_error("Plik " PROJECT_SHA " ma niewłaściwą długość"));
    Task::session_data["project_sha"] = value;
  }else{
    log("Nie znalazłem pliku" PROJECT_SHA);
  }
  Task::sha_data_initialized = true;
}

void set_project_name(string project){
  Task::session_data["project"] = project;
}

void get_user_data(string instr){
  if(!Task::user_data_initialized){
    log("Pobieram dane osobowe");
    Instruction in(instr);
    Userdata ud;
    for(auto& v : {"name", "gender"})
      Task::session_data[v] = ud.data[v];
    Task::session_data["age"] = stoi(ud.data["age"]);
    Task::user_data_initialized = true;
  }
}

string get_random_condition(string task_name, vector<string> conditions){
  // liczebności również warunków, które jeszcze nie wystąpiły w bazie
  // (wtedy = 0)
  map<string, int> counts;
  for(auto c : conditions)
    counts[c] = 0;
  auto res = Task::db.query("SELECT cnd, COUNT(*) FROM session WHERE task = '" + task_name +
                            "' AND stage = 'finished' AND name != 'admin' GROUP BY cnd;");
  while(res->next()){
    counts[res->getString(1)] = res->getInt(2);
    conditions.push_back(res->getString(1));
  }
  
  log("Liczba sesji na warunek " + task_name + ":");
  for(auto c : counts)
    log(c.first + ": " + to_string(c.second));

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
  Task::session_data["cnd"] = chosen;
  log("Wybrany warunek: " + chosen);
  return chosen;
}

Ptype Task::get_session_data(string name){
  return session_data[name];
}

map<string, vector<string> > Task::get_unfinished_sessions(string taskname, map<string, Ptype>& session_data){
    map<string, Ptype> data;
    for(auto& v : {"name", "age", "gender"})
      data[v] = session_data[v];
    auto res = Task::db.query("SELECT session_id, cnd, time FROM session WHERE stage = 'started' AND task = '" + taskname +
                              "' AND " + Task::db.match_statement(data) + ";");
    map<string, vector<string> > tbl;
    while(res->next())
      for(auto& v : {"session_id", "cnd", "time"})
        tbl[v].push_back(res->getString(v));
    return tbl;
}

void Task::register_session(){
  if(user_data_initialized){
    log("Rejestruję sesję");
    Task::db.execute(Task::db.insert_statement("session", session_data));
    auto r = Task::db.query("SELECT LAST_INSERT_ID();");
    if(r->next())
      session_id = r->getInt(1);
  }else{
    throw(runtime_error("Brak danych osobowych, nie mogę zarejestrować tej sesji."));
  }
}

bool Task::measure_key_reaction(const vector<int>& response_keys, int& response, int& rt, const time_type& start){
  for(int i = 0; i < response_keys.size(); i++){
    rt = keyp(response_keys[i]) - start;
    if(rt > 0){
      response = i;
      return true;
    }
  }
  return false;
}

bool Task::task_is_finished(){
  if(!finished)
    finished = (current_trial == nof_trials) || ((max_task_time > 0) && ((time_ms() - task_start) >= max_task_time));
  return finished;
}

void Task::mark_session_finished(){
  Task::db.execute("UPDATE session SET stage = \"finished\" WHERE session_id = " + to_string(session_id) + ";");
}

void Task::run(string task_name, vector<pair<string, vector<Ptype> > > design,
               unsigned int b, unsigned int n, unsigned int nof_trials,
               unsigned int max_task_time){
  init(task_name, design, b, n, nof_trials, max_task_time);
  run();
}

void Task::init(string task_name_, vector<pair<string, vector<Ptype> > > design_,
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
  session_data["task"] = task_name;

  initialized = true;
  finished = false;
}

void Task::run(){
  if(!initialized)
    throw(runtime_error("Wywołano funkcję run, ale zadanie nie zostało zainicjalizowane"));

  srand(time(NULL));

  cs = unique_ptr<Conditions>(new Conditions(design));
  if(nof_trials == 0){
    nof_trials = cs->nof_cnds * b * n;
  }else if(nof_trials > (cs->nof_cnds * b * n)){
    b = ceil((float)nof_trials / (cs->nof_cnds * n));
  }
  scen = unique_ptr<Scenario>(new Scenario(cs->nof_cnds, b, n));

  log("Długość zadania: " + to_string(nof_trials));
  cs->print();

  get_sha_data();
  if(use_db){
    Task::db.connect();
    register_session();
  }

  Media::init();

  log("Rozpoczynam pętlę prób zadania");
  task_start = time_ms();
  for(current_trial = 0; !task_is_finished(); current_trial++){

    trial_data.clear();
    trial_data["trial"] = current_trial;
    trial_data["cnd_num"] = (int)scen->get(current_trial);
    for(auto& f : cs->names)
      trial_data[f] = cnd(f);
    unique_ptr<Datasaver> data_saver;
    
    TRIAL_IS_OVER = false;
    set_state(0);
    while(!TRIAL_IS_OVER && (keyp(KEYESCAPE) <= task_start)){
      process_events(event);
      set_active();
      trial_code(state());
    }
    log("Próba zakończona");

    if(keyp(KEYESCAPE) <= task_start){
      if(use_db){
        log("Zapisuję dane");
        data_saver = unique_ptr<Datasaver>(new Datasaver(&db, task_name, session_id, session_data, trial_data));
      }else{
        string msg = "Dane z próby:\n";
        for(auto& d : trial_data)
          msg += d.first + ": " + (string)d.second + " ";
        log(msg);
      }
    }else{
      break;
    }
  }

  if(use_db){
    if(task_is_finished())
      mark_session_finished();
    Task::db.disconnect();
  }

  Media::close();
  
  log("Zadanie trwało " + to_string(floor((time_ms() - task_start) / 60000)) + " minut");
}
