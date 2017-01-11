// -*- coding: utf-8 -*-

#include "Task.hpp"

// Elementy wspólne dla wszystkich zadań
Database Task::db;
mutex Task::settings_mutex;
map<string, string> Task::settings;
map<string, Ptype> Task::session_data;
#define SESSION_ID_UNINITIALIZED -1
int Task::session_id = SESSION_ID_UNINITIALIZED;
bool Task::user_data_initialized = false;

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

// void get_sha_data(){
//   if(Task::sha_data_initialized)
//     return;
//   ifstream f;
//   f.open(LIB_SHA);
//   string value;
//   if(f.good()){
//     getline(f, value);
//     if(value.size() != 40)
//       throw(runtime_error("Plik " LIB_SHA " ma niewłaściwą długość"));
//     Task::session_data["lib_sha"] = value;
//   }else{
//     log("Nie znalazłem pliku " LIB_SHA ".");
//   }
//   f.close();
//   f.open(PROJECT_SHA);
//   if(f.good()){
//     getline(f, value);
//     if(value.size() != 40)
//       throw(runtime_error("Plik " PROJECT_SHA " ma niewłaściwą długość"));
//     Task::session_data["project_sha"] = value;
//   }else{
//     log("Nie znalazłem pliku" PROJECT_SHA);
//   }
//   Task::sha_data_initialized = true;
// }

void set_project_name(string project){
  log("Nazwa projektu: " + project);
  Task::session_data["project"] = project;
}

void get_user_data(string instr){
  if(!Task::user_data_initialized){
    log("Pobieram dane osobowe");
    Instruction in(instr);
    Userdata ud;
    for(auto& v : {"subject", "gender"})
      Task::session_data[v] = ud.data[v];
    Task::session_data["age"] = stoi(ud.data["age"]);
    Task::user_data_initialized = true;
  }else{
    log("Dane osobowe są utalone");
  }
}

string get_random_condition(vector<string> conditions){
  if(Task::session_data.count("project") != 1)
    throw(runtime_error("Próba ustalenia warunku losowego bez ustalonej nazwy projektu"));
  auto project = (string)Task::session_data["project"];
  log("Ustalam losowy warunek");
  // liczebności również warunków, które jeszcze nie wystąpiły w bazie
  // (wtedy = 0)
  map<string, int> counts;
  for(auto c : conditions)
    counts[c] = 0;
  auto res = Task::db.query("SELECT cnd, COUNT(*), FROM session WHERE project = '" + project + "' "
                            "AND stage = 'finished' "
                            // "AND tag = '" + (session_data.count("tag") == 1 ? session_data["tag"] : "") + "' "
                            "AND subject != 'admin' GROUP BY cnd;");
  while(res->next()){
    counts[res->getString(1)] = res->getInt(2);
    conditions.push_back(res->getString(1));
  }
  
  log("Liczba sesji na warunek " + project + ":");
  for(auto c : counts)
    log(c.first + ": " + to_string(c.second));

  string chosen;
  if(Task::session_data["subject"] == "admin"){
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


// Najpierw musimy ustalić, czy taka sama osoba wykonywała 1+ razy to
// samo zadanie w tym samym projekcie, sprawdzamy, też, ile prób
// aktualnego zadania wykonała w każdej z tych sesji.
void Task::get_unfinished_sessions(){
    map<string, Ptype> match_data;
    for(auto& v : {"subject", "age", "gender", "project"})
      match_data[v] = Task::session_data[v];
    // Zbieramy dane o wszystkich nieskończonych sesjach tej osoby w tym projekcie
    auto res = Task::db.query("SELECT session_id, cnd, timestamp FROM session WHERE " + Task::db.match_statement(match_data) + " "
                              " AND stage = 'started';");
    vector<int> session_ids;
    map<int, map<string, string> > sessions;
    while(res->next())
      for(auto& v : {"cnd"}){
        session_ids.push_back(res->getInt("session_id"));
        sessions[res->getInt("session_id")][v] = res->getString(v);
      }
    // Dla każdej niezakończonej sesji zbieramy dane o liczbie prób
    // niezakończonego zadania
    for(auto& sid : session_ids){
      res = Task::db.query("SELECT session_tables.timestamp, count(*) as count FROM "
                           "session_tables JOIN " + table_name + " USING session_id "
                           " WHERE session_id = " + to_string(sid) + " "
                           "AND session_tables.stage = 'started' GROUP BY session_id;");
      // while(res->next())
      //   sessions[sid][]
    }
}

void Task::register_session(){
  if(user_data_initialized){
    if(session_id != SESSION_ID_UNINITIALIZED){
      log("Sesja jest już zarejestrowana");
      return;
    }
    if(session_data.count("project") == 0)
      throw(runtime_error("Próba zarejestrowania sesji bez podania nazwy projektu"));
    log("Rejestruję sesję");
    Task::db.execute(Task::db.insert_statement("session", session_data));
    auto r = Task::db.query("SELECT LAST_INSERT_ID();");
    if(r->next())
      session_id = r->getInt(1);
  }else{
    throw(runtime_error("Brak danych osobowych, nie mogę zarejestrować tej sesji."));
  }
}

void Task::register_task(){
  log("Rejestruję zadanie");
  Task::db.execute(Task::db.insert_statement("session_tables", {{"session_id", session_id},
          {"table_name", table_name}, {"stage", "started"}}));
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

void Task::mark_task_finished(){
  log("Zmieniam status zadania na ukończone");
  Task::db.execute("UPDATE session_tables SET stage = \"finished\" WHERE table_name = '" + table_name + "'"
                   " AND session_id = " + to_string(session_id) + ";");
}

void update_session_status(vector<string> table_names){
  bool session_finished = true;
  log("Ustalam status sesji");
  for(auto& table_name : table_names){
    auto res = Task::db.query("SELECT stage FROM session_tables WHERE session_id = " + to_string(Task::session_id) + ";");
    if(res->next()){
      if(res->getString(1) != "finished")
        session_finished = false;
    }else{
      session_finished = false;
    }
  }
  if(session_finished){
    log("Zmieniam status sesji na ukończona");
    Task::db.execute("UPDATE session SET stage = \"finished\" WHERE session_id = " + to_string(Task::session_id) + ";");
  }else{
    log("Sesja nie została ukończona");
  }
}

void Task::run(string table_name, vector<pair<string, vector<Ptype> > > design,
               unsigned int b, unsigned int n, unsigned int nof_trials,
               unsigned int max_task_time){
  init(table_name, design, b, n, nof_trials, max_task_time);
  run();
}

void Task::init(string table_name_, vector<pair<string, vector<Ptype> > > design_,
                unsigned int b_, unsigned int n_, unsigned int nof_trials_,
                unsigned int max_task_time_){
  table_name = table_name_; design = design_;
  b = b_; n = n_; nof_trials = nof_trials_;
  max_task_time = max_task_time_;
  log("Inicjalizuję zadanie");

  if(getenv("TASKLIB_NODB") != nullptr)
    use_db = false;

  if(getenv("TASKLIB_DEBUG") != nullptr)
    debug = true;

  if(table_name == "")
    throw(runtime_error("Nie podano nazwy tabeli danych"));

  initialized = true;
  finished = false;
}

void Task::run(){
  if(!initialized)
    throw(runtime_error("Wywołano funkcję run, ale zadanie nie zostało zainicjalizowane"));
  log("Uruchamiam zadanie");
  
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

  session_data["lib_sha"] = lib_sha;
  session_data["project_sha"] = project_sha;
  if(use_db){
    Task::db.connect();
    if(session_id == SESSION_ID_UNINITIALIZED)
      register_session();
    register_task();
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
      // set_active();
      trial_code(state());
      process_events(event);
    }
    log("Próba zakończona");

    if(keyp(KEYESCAPE) <= task_start){
      if(use_db){
        log("Zapisuję dane");
        data_saver = unique_ptr<Datasaver>(new Datasaver(&db, table_name, session_id, session_data, trial_data));
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
      mark_task_finished();
    Task::db.disconnect();
  }

  Media::close();
  
  log("Zadanie trwało " + to_string(floor((time_ms() - task_start) / 60000)) + " minut");
}
