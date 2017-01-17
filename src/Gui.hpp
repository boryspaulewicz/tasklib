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

//! Przerobić na wersję z pojedynczym widocznym pytaniem, albo dodać zaznaczanie
// brakujących odpowiedzi.

class QuestionnaireItem : public TaskWindow {
    using ButtonType = Gtk::RadioButton;
private:

    TextView2 tv_instruction;
    vector<Label*> tv_questions;
    vector<RadioButtonGroup> groups;
    vector<vector<ButtonType*>> answer_buttons;
    Gtk::Button ok_button;

    void answer_on_clicked(int question, int v) {
        value[question] = v;
    }

    void ok_button_clicked() {
        bool all_answered = true;
        for (int i = 0; i < value.size(); i++)
            if (value[i] == -1) {
                all_answered = false;
                break;
            }
        if (all_answered) {
            close();
        } else {
            MessageDialog msg(*this, "Nie podano wszystkich odpowiedzi");
            msg.run();
        }
    }

public:

    vector<int> value;

    QuestionnaireItem(string instruction, vector<string> questions,
            vector<string> answers, float width = 0.5, float height = 0.9) :
    ok_button("Dalej") {
        init();
        set_default_size(get_screen()->get_width() * width,
                get_screen()->get_height() * height);

        value.resize(questions.size(), 0);
        for (int q = 0; q < questions.size(); q++) {
            //            TextView2* tv = new TextView2();
            //            tv->set_text(questions.at(q));
            //            tv_questions.push_back(tv);
            Label* l = new Label(questions[q]);
            tv_questions.push_back(l);
            {
                vector<ButtonType*> vrb;
                answer_buttons.push_back(vrb);
            }
            // Niewidoczny pierwszy przycisk, który definiuje grupę
            ButtonType* rb = new ButtonType("");
            groups.push_back(rb->get_group());
            for (int a = 0; a < answers.size(); a++) {
                ButtonType* rb = new ButtonType(answers[a]);
                rb->signal_clicked().connect(sigc::bind<int, int>(sigc::mem_fun(*this,
                        &QuestionnaireItem::answer_on_clicked), q, a));
                rb->set_group(groups[q]);
                answer_buttons.at(q).push_back(rb);
            }
            // Dodawanie przycisków do grupy wywołuje sygnał clicked.
            value[q] = -1;
        }

        VBox vbox;
        frame.add(vbox);
        tv_instruction.set_text(instruction);
        tv_instruction.set_border_width(10);
        vbox.pack_start(tv_instruction, false, false);
        ScrolledWindow sw;
        vbox.pack_start(sw, true, true);
        VBox vbox2;
        sw.add(vbox2);
        vbox.pack_start(vbox2);
        for (int q = 0; q < questions.size(); q++) {
            HBox* hbox = new HBox;
            vbox2.pack_start(*hbox);
            hbox->pack_start(*tv_questions[q], false, false);
            Label* l = new Label("");
            vbox2.pack_start(*l, false, false);
            hbox = new HBox;
            vbox2.pack_start(*hbox);
            for (int a = 0; a < answers.size(); a++) {
                hbox->pack_start(*answer_buttons[q][a], false, false);
            }
        }
        HBox hbox2;
        hbox2.set_border_width(10);
        vbox.pack_end(hbox2, false, false);
        hbox2.pack_end(ok_button, true, true);
        ok_button.signal_clicked().connect(sigc::mem_fun(*this,
                &QuestionnaireItem::ok_button_clicked));

        cout << "Uruchamiam qi" << endl;
        run();
    }

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
