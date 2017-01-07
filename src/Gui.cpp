// coding: utf-8 -*-

#include "Gui.hpp"
#include "Utils.hpp"

// Ten obiekt musi powstać przed wszystkimi innymi
namespace Gui{
  Main m;
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

Textview::Textview(){
    Pango::FontDescription fd;
    fd.set_size(15 * Pango::SCALE);
    override_font(fd);
    property_cursor_visible() = false;
    set_wrap_mode(WRAP_WORD);
    set_left_margin(10);
    set_right_margin(10);
    set_justification(JUSTIFY_FILL);
}

void Textview::set_text(string text){
  get_buffer()->set_text(text);
}

string load_from_file(string fname){
  ifstream f;
  f.open(fname);
  if(!f.good()){
    throw(runtime_error("Nie udało się otworzyć pliku " + fname));
    exit(1);
  }
  stringstream contents;
  contents << f.rdbuf();
  return contents.str();
}

void Userdata::button_pressed(){
  if(trim(name.get_text()) == string("")){
    MessageDialog msg(*this, "Brak danych w polu Identyfikator");
    msg.run();
    return;
  }
  if(trim(name.get_text()) == "admin"){
    data["name"] = "admin";
    data["age"] = "0";
    data["gender"] = "M";
    log("Podano identyfikator administratora");
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
  data["name"] = trim(name.get_text());

  if(trim(age.get_text()) == string("")){
    MessageDialog msg(*this, "Brak danych w polu Wiek");
    msg.run();
    return;
  }
  if(!rmatch("^ *[0-9][0-9] *$", age.get_text())){
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
  data["gender"] = gender.get_active_text(); // nullptr jeśli żaden nie jest wybrany

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
  for(auto e : valuee){
    value.push_back(trim(e->get_text()));
  }
  close();
  return;
}

Uservalue::Uservalue(vector<string> labels, bool visibility) : button("Ok"){
  init();
  
  frame.add(vbox);
  vbox.set_border_width(10);
  vbox.set_spacing(10);
  for(string label : labels){
    valuel.push_back(new Label(label));
    valuee.push_back(new Entry());
    vbox.pack_start(*valuel[valuel.size() - 1]);
    vbox.pack_start(*valuee[valuee.size() - 1]);
    valuee[valuee.size() - 1]->set_visibility(visibility);
  }
  vbox.pack_start(button);

  button.signal_clicked().connect(sigc::mem_fun(*this, &Uservalue::button_pressed));

  run();
  for(auto l : valuel)
    delete l;
  for(auto e : valuee)
    delete e;
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
  
Instruction::Instruction(string contents, vector<string> labels, float width, float height){
  tv.set_text(contents);

  init();
  set_default_size(get_screen()->get_width() * width, get_screen()->get_height() * height);

  frame.add(vbox);
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

