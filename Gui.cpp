// coding: utf-8 -*-

#include "Gui.hpp"

// Ten obiekt musi powstać przed wszystkimi innymi
namespace Gui{
  Main m;
}

string trim(string str)
{
  size_t first = str.find_first_not_of(' ');
  if (first == string::npos)
    return "";
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last-first+1));
}

bool rmatch(string pattern, string str){
  smatch m;
  regex re(pattern);
  return regex_match(str, m, re);
}

void Taskwindow::init(){
  property_decorated() = false;
  set_position(Gtk::WIN_POS_CENTER_ALWAYS);
  set_border_width(10);
  add(frame);
}

void Taskwindow::run(){
  show_all_children();
  Gui::m.run(*this);
}

void Userdata::button_pressed(){
  if(trim(name.get_text()) == string("")){
    MessageDialog msg(*this, "Brak danych w polu Identyfikator");
    msg.run();
    return;
  }
  if(trim(name.get_text()) == "admin"){
    data["name"] = "'admin'";
    data["age"] = "0";
    data["gender"] = "'M'";
    cout << "Admin user data provided" << endl;
    close();
    return;
  }
  if(!rmatch("^ *\\w\\w[0-3][0-9][0-1][0-9] *$", name.get_text())){
    MessageDialog msg(*this, "Błędne dane w polu Identyfikator:\n"
                      "Podaj swoje inicjały, dwucyfrowy dzień i dwucyfrowy miesiąc urodzenia,\n"
                      "na przykład: jk0112");
    msg.run();
    return;
  }
  data["name"] = "'" + trim(name.get_text()) + "'";

  if(trim(age.get_text()) == string("")){
    MessageDialog msg(*this, "Brak danych w polu Wiek");
    msg.run();
    return;
  }
  if(!rmatch("^ *[1-9][0-9] *$", age.get_text())){
    MessageDialog msg(*this, "Błędna wartość w polu wiek: " + age.get_text());
    msg.run();
    return;
  }
  data["age"] = trim(age.get_text());

  if(gender.get_active_text() == string("")){
    MessageDialog msg(*this, "Nie wybrano płci");
    msg.run();
    return;
  }
  data["gender"] = "'" + gender.get_active_text() + "'"; // nullptr jeśli żaden nie jest wybrany

  close();
}

Userdata::Userdata() : button("Dalej"), namel("Identyfikator"), agel("Wiek"), genderl("Płeć"){
  init();

  frame.add(vbox);
  vbox.set_border_width(10);
  vbox.set_spacing(10);
  vbox.pack_start(namel);
  vbox.pack_start(name);
  vbox.pack_start(agel);
  vbox.pack_start(age);
  vbox.pack_start(genderl);
  for(auto g : {"K", "M"})
    gender.append(g);
  vbox.pack_start(gender);
  vbox.pack_start(button);

  button.signal_clicked().connect(sigc::mem_fun(*this, &Userdata::button_pressed));

  run();
}

void Uservalue::button_pressed(){
  if(trim(valuee.get_text()) != ""){
    value = trim(valuee.get_text());
    close();
  }else{
    MessageDialog msg(*this, "Nie podano żadnej wartości");
    msg.run();
    return;
  }
}

Uservalue::Uservalue(string label, bool visibility) : valuel(label), button("Ok"){
  init();
  
  frame.add(vbox);
  vbox.set_border_width(10);
  vbox.set_spacing(10);
  vbox.pack_start(valuel);
  vbox.pack_start(valuee);
  valuee.set_visibility(visibility);
  vbox.pack_start(button);

  button.signal_clicked().connect(sigc::mem_fun(*this, &Uservalue::button_pressed));

  run();
}
  

void VButton::on_clicked(){
  value->assign(get_label());
  parent->close();
}
  
VButton::VButton(string label, string* value, Window* parent) : Button(label), value(value), parent(parent){ }

Instruction::~Instruction(){
  for(auto b : buttons)
    delete b;
}
  
Instruction::Instruction(string fname, initializer_list<string> labels, float width, float height){
  ifstream f;
  f.open(fname);
  if(!f.good()){
    throw(runtime_error("Nie udało się otworzyć pliku " + fname));
    exit(1);
  }
  stringstream contents;
  contents << f.rdbuf();

  init();
  set_default_size(get_screen()->get_width() * width, get_screen()->get_height() * height);

  frame.add(vbox);
  tv.get_buffer()->set_text(contents.str());
  tv.property_cursor_visible() = false;
  tv.set_wrap_mode(WRAP_WORD);
  tv.set_left_margin(10);
  tv.set_right_margin(10);
  tv.set_justification(JUSTIFY_FILL);
  vbox.set_spacing(10);
  vbox.set_border_width(10);
  vbox.pack_start(tv);
  vbox.pack_start(hbox, false, false);
  for(auto l : labels)
    buttons.push_back(new VButton(l, &value, this));
  hbox.set_spacing(10);
  for(auto b : buttons)
    hbox.pack_end(*b, false, false);

  run();
}

void Chooseitem::button_pressed(){
  if(cbox.get_active_text() != string("")){
    value = cbox.get_active_text();
    close();
  }else{
    MessageDialog msg(*this, "Nie wybrano żadnej wartości");
    msg.run();
    return;
  }
}

Chooseitem::Chooseitem(vector<string> values, string text) : label(text), button("Ok"){
  init();

  frame.add(vbox);
  vbox.set_border_width(10);
  vbox.set_spacing(10);
  vbox.pack_start(label);
  for(auto v : values)
    cbox.append(v);
  vbox.pack_start(cbox);
  vbox.pack_start(button);

  button.signal_clicked().connect(sigc::mem_fun(*this, &Chooseitem::button_pressed));

  run();
}

