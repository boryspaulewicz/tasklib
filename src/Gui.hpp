// -*- coding: utf-8 -*-

#ifndef GUI
#define GUI

#include "Common.hpp"
#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/textview.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/scrolledwindow.h>
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

string string_from_file(string fname);

namespace Gui {
    extern Main m;
}

class TaskWindow : public Gtk::Window {
public:

    Frame frame;

    void init();

    void run();

};

class TextView2 : public TextView {
public:

    TextView2();

    void set_text(string text);

};

class QuestionnaireItem : public TaskWindow {
    using ButtonType = Gtk::RadioButton;
private:

    TextView2 tv_instruction;
    vector<Label*> lbl_questions;
    vector<RadioButtonGroup> answer_groups;
    vector<vector<ButtonType*>> answer_buttons;
    vector<Frame*> question_frames;
    Gtk::Button ok_button;

    void answer_on_clicked(int question, int v);

    void ok_button_clicked();
    
public:

    vector<int> value;

    QuestionnaireItem(string instruction, vector<string> questions,
            vector<string> answers, float width = 0.5, float height = 0.9);

};

class UserData : public TaskWindow {
    VBox vbox;
    Label namel, agel, genderl;
    Entry name, age;
    ComboBoxText gender;
    Button button;

    void button_pressed();

public:

    std::map<string, string> data;

    UserData();

};

class UserValue : public TaskWindow {
    string reg, message;
    VBox vbox;
    vector<Label*> valuel;
    vector<Entry*> valuee;
    Button button;

    void button_pressed();

public:

    vector<string> value;

    UserValue(vector<string> labels, bool visibility = true, string reg = "", string msg = "");

};

class VButton : public Button {
    string* value;
    Gtk::Window *parent;

public:

    virtual void on_clicked();

    VButton(string label, string* value, Gtk::Window* parent);

};

class Instruction : public TaskWindow { // Gtk::Window{

    VBox vbox;
    HBox hbox;
    TextView2 tv;
    vector<VButton*> buttons;

public:

    string value;

    Instruction(string contents, vector<string> labels = {"Dalej"}, float width = .6, float height = .8);

    ~Instruction();

};

class ChooseItem : public TaskWindow {
    VBox vbox;
    Label label;
    ComboBoxText cbox;
    Button button;

    void button_pressed();

public:

    string value;

    ChooseItem(vector<string> values, string text = "Wybierz wartość");

};

#endif
