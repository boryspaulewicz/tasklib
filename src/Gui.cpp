// coding: utf-8 -*-

#include "Gui.hpp"
#include "Utils.hpp"

// Ten obiekt musi powstać przed wszystkimi innymi
namespace Gui {
    Main m;
}

void TaskWindow::init() {
    property_decorated() = false;
    // set_modal(true);
    set_keep_above(true);
    set_position(Gtk::WIN_POS_CENTER_ALWAYS);
    set_border_width(10);
    add(frame);
}

void TaskWindow::run() {
    show_all_children();
    Gui::m.run(*this);
}

TextView2::TextView2() {
    Pango::FontDescription fd;
    fd.set_size(15 * Pango::SCALE);
    override_font(fd);
    property_cursor_visible() = false;
    set_wrap_mode(WRAP_WORD);
    set_left_margin(10);
    set_right_margin(10);
    set_justification(JUSTIFY_FILL);
}

void TextView2::set_text(string text) {
    get_buffer()->set_text(text);
}

void QuestionnaireItem::answer_on_clicked(int question, int v){
    value[question] = v;
}

void QuestionnaireItem::ok_button_clicked(){
        bool all_answered = true;
        for (int i = 0; i < value.size(); i++)
            if (value[i] == -1) {
                all_answered = false;
            }else{
                question_frames[i]->set_visible(false); // setset_opacity(.5);
            }
        if (all_answered) {
            close();
        } else {
            MessageDialog msg(*this, "Proszę odpowiedzieć na wszystkie pytania");
            msg.run();
        }
}

QuestionnaireItem::QuestionnaireItem(string instruction, vector<string> questions, vector<string> answers, float width, float height) :
ok_button("Dalej") {
    init();
    set_default_size(get_screen()->get_width() * width,
            get_screen()->get_height() * height);

    value.resize(questions.size(), 0);
    for (int q = 0; q < questions.size(); q++) {
        Label* l = new Label(questions[q]);
        lbl_questions.push_back(l);
        {
            vector<ButtonType*> vrb;
            answer_buttons.push_back(vrb);
        }
        // Niewidoczny pierwszy przycisk, który definiuje grupę
        ButtonType* rb = new ButtonType("");
        answer_groups.push_back(rb->get_group());
        for (int a = 0; a < answers.size(); a++) {
            ButtonType* rb = new ButtonType(answers[a]);
            rb->signal_clicked().connect(sigc::bind<int, int>(sigc::mem_fun(*this,
                    &QuestionnaireItem::answer_on_clicked), q, a));
            rb->set_group(answer_groups[q]);
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
    VBox questions_vbox;
    sw.add(questions_vbox);
    vbox.pack_start(questions_vbox);
    for (int q = 0; q < questions.size(); q++) {
        Frame* question_frame = new Frame;
        question_frames.push_back(question_frame);
        questions_vbox.pack_start(*question_frame, false, false);
        VBox* question_vbox = new VBox;
        question_frame->add(*question_vbox);
        // Pytanie
        HBox* question_hbox = new HBox;
        question_hbox->set_border_width(5);
        question_vbox->pack_start(*question_hbox);
        question_hbox->pack_start(*lbl_questions[q], false, false);
        // Odpowiedzi
        HBox* items_hbox = new HBox;
        items_hbox->set_border_width(5);
        question_vbox->pack_start(*items_hbox);
        for (int a = 0; a < answers.size(); a++) {
            items_hbox->pack_start(*answer_buttons[q][a], false, false);
        }
    }
    HBox ok_button_hbox;
    ok_button_hbox.set_border_width(10);
    vbox.pack_end(ok_button_hbox, false, false);
    ok_button_hbox.pack_end(ok_button, false, false);
    ok_button.signal_clicked().connect(sigc::mem_fun(*this,
            &QuestionnaireItem::ok_button_clicked));

    run();
}

void UserData::button_pressed() {
    if (trim(name.get_text()) == string("")) {
        MessageDialog msg(*this, "Brak danych w polu Identyfikator");
        msg.run();
        return;
    }
    if (trim(name.get_text()) == "admin") {
        data["subject"] = "admin";
        data["age"] = trim(age.get_text()) == "" ? "0" : trim(age.get_text());
        data["gender"] = gender.get_active_text() == "" ? "M" : gender.get_active_text();
        log("Podano identyfikator administratora");
        close();
        return;
    }
    if (!rmatch("^ *\\w\\w[0-3][0-9][0-1][0-9] *$", name.get_text())) {
        MessageDialog msg(*this, "Błędne dane w polu Identyfikator:\n"
                "Podaj swoje inicjały, dwucyfrowy dzień i dwucyfrowy miesiąc urodzenia,\n"
                "na przykład: jk0112");
        msg.run();
        return;
    }
    data["subject"] = tolower(trim(name.get_text()));

    if (trim(age.get_text()) == string("")) {
        MessageDialog msg(*this, "Brak danych w polu Wiek");
        msg.run();
        return;
    }
    if (!rmatch("^ *[0-9][0-9] *$", age.get_text())) {
        MessageDialog msg(*this, "Błędna wartość w polu wiek: " + age.get_text());
        msg.run();
        return;
    }
    data["age"] = trim(age.get_text());

    if (gender.get_active_text() == string("")) {
        MessageDialog msg(*this, "Nie wybrano płci");
        msg.run();
        return;
    }
    data["gender"] = gender.get_active_text(); // nullptr jeśli żaden nie jest wybrany

    close();
}

UserData::UserData() : button("Dalej"), namel("Identyfikator"), agel("Wiek"), genderl("Płeć") {
    init();

    frame.add(vbox);
    vbox.set_border_width(10);
    vbox.set_spacing(10);
    vbox.pack_start(namel);
    vbox.pack_start(name);
    vbox.pack_start(agel);
    vbox.pack_start(age);
    vbox.pack_start(genderl);
    for (auto g :{"K", "M"})
        gender.append(g);
    vbox.pack_start(gender);
    vbox.pack_start(button);

    button.signal_clicked().connect(sigc::mem_fun(*this, &UserData::button_pressed));

    run();
}

void UserValue::button_pressed() {
    bool all_fine = true;
    value.clear();
    for (auto e : valuee) {
        if (reg != "") {
            if (!rmatch(reg, e->get_text())) {
                MessageDialog msg(*this, ("Błędne dane:\n" + message).c_str());
                msg.run();
                all_fine = false;
                break;
            }
        }
        value.push_back(trim(e->get_text()));
    }
    if (all_fine)
        close();
    return;
}

UserValue::UserValue(vector<string> labels, bool visibility, string reg, string msg) : button("Ok"),
message{msg},
reg{reg}
{
    init();

    frame.add(vbox);
    vbox.set_border_width(10);
    vbox.set_spacing(10);
    for (string label : labels) {
        valuel.push_back(new Label(label));
        valuee.push_back(new Entry());
        vbox.pack_start(*valuel[valuel.size() - 1]);
        vbox.pack_start(*valuee[valuee.size() - 1]);
        valuee[valuee.size() - 1]->set_visibility(visibility);
    }
    vbox.pack_start(button);

    button.signal_clicked().connect(sigc::mem_fun(*this, &UserValue::button_pressed));

    run();
    for (auto l : valuel)
        delete l;
    for (auto e : valuee)
        delete e;
}

void VButton::on_clicked() {
    value->assign(get_label());
    parent->close();
}

VButton::VButton(string label, string* value, Window* parent) : Button(label), value(value), parent(parent) {
}

Instruction::~Instruction() {
    for (auto b : buttons)
        delete b;
}

Instruction::Instruction(string contents, vector<string> labels, float width, float height) {
    tv.set_text(contents);

    init();
    set_default_size(get_screen()->get_width() * width, get_screen()->get_height() * height);

    frame.add(vbox);
    vbox.set_spacing(10);
    vbox.set_border_width(10);
    vbox.pack_start(tv);
    vbox.pack_start(hbox, false, false);
    for (auto l : labels)
        buttons.push_back(new VButton(l, &value, this));
    hbox.set_spacing(10);
    for (auto b : buttons)
        hbox.pack_end(*b, false, false);

    run();
}

void ChooseItem::button_pressed() {
    if (cbox.get_active_text() != string("")) {
        value = cbox.get_active_text();
        close();
    } else {
        MessageDialog msg(*this, "Nie wybrano żadnej wartości");
        msg.run();
        return;
    }
}

ChooseItem::ChooseItem(vector<string> values, string text) : label(text), button("Ok") {
    init();

    frame.add(vbox);
    vbox.set_border_width(10);
    vbox.set_spacing(10);
    vbox.pack_start(label);
    for (auto v : values)
        cbox.append(v);
    vbox.pack_start(cbox);
    vbox.pack_start(button);

    button.signal_clicked().connect(sigc::mem_fun(*this, &ChooseItem::button_pressed));

    run();
}

