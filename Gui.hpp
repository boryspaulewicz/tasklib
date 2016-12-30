// -*- coding: utf-8 -*-

#ifndef GUI
#define GUI

#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/textview.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/messagedialog.h>
#include <pangomm/fontdescription.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
using namespace std;
using namespace Gtk;
using namespace Glib;

string trim(string str);

bool rmatch(string pattern, string str);

class Taskwindow : public Gtk::Window{
public:

  Frame frame;
  
  void init();

  void run();

};

class Userdata : public Taskwindow{
  
  VBox vbox;
  Label namel, agel, genderl;
  Entry name, age;
  ComboBoxText gender;
  Button button;

  void button_pressed();

public:

  std::map<string, string> data;
  
  Userdata();

};

class Uservalue : public Taskwindow{

  VBox vbox;
  Label valuel;
  Entry valuee;
  Button button;

  void button_pressed();
  
public:

  string value;

  Uservalue(string label, bool visibility = true);
  
};

class VButton : public Button{

  string* value;
  Gtk::Window *parent;

public:

  virtual void on_clicked();

  VButton(string label, string* value, Gtk::Window* parent);

};

class Instruction : public Taskwindow{ // Gtk::Window{
  
  VBox vbox;
  HBox hbox;
  vector<VButton*> buttons;
  
public:

  string value;
  TextView tv;
  
  Instruction(string fname, initializer_list<string> labels = {"Dalej"}, float width = .6, float height = .8);

  ~Instruction();

};

class Chooseitem : public Taskwindow{

  VBox vbox;
  Label label;
  ComboBoxText cbox;
  Button button;

  void button_pressed();
  
public:

  string value;

  Chooseitem(vector<string> values, string text = "Wybierz wartość");

};

#endif
