// -*- coding: utf-8 -*-

//! TODO dodać przerwę, przerobić main na elegantszy i bardziej wzorcowy

#include "Task.hpp"

#define FIXATION_TIME 1000
#define PRE_FIXATION_TIME 500

class SRT : public Task{
    public:
                        
        void draw_text(string s, float x = .5, float y = .5){
            text.setString(utf32(s));
            center(text);
            text.setPosition(Vector2f(width * x, height * y));
            draw(text);
        }
        
        time_type rt;
        int intensity, delay;
        
        enum stage{START, WFK, PRE_FIXATION, DRAW_FIXATION, FIXATION, CLEAR_FIXATION, BLANK, DRAW_STIM, STIM, FEEDBACK, FINAL_CLEAR};
        
        void trial_code(int state){
            switch(state){
                
                case START:
                    intensity = random_int(10, 255);
                    delay = random_int(1000, 2000);
                    set_state(PRE_FIXATION);
                    return;
                    
                case PRE_FIXATION:
                    clear(bg);
                    display();
                    if(state_time() > PRE_FIXATION_TIME)
                        if(current_trial == 0){
                            set_state(WFK);
                        }else{
                            set_state(DRAW_FIXATION);
                        }
                    return;
                    
                case WFK:
                    if(some_keyp() <= state_start()){
                        clear(bg);
                        draw_text("Naciśnij dowolny klawisz, aby rozpocząć zadanie");
                        display();
                    }else{
                        set_state(DRAW_FIXATION);
                    }
                    return;
                                        
                case DRAW_FIXATION:
                    clear(bg);
                    draw_text(to_string(current_trial + 1) + "/" + to_string(nof_trials));
                    display();
                    set_state(FIXATION);
                    return;
                    
                case FIXATION:
                    if(state_time() >= FIXATION_TIME)
                        set_state(CLEAR_FIXATION);
                    return;
                    
                case CLEAR_FIXATION:
                    clear(bg);
                    display();
                    set_state(BLANK);
                    return;
                    
                case BLANK:
                    if(state_time() >= delay)
                        set_state(DRAW_STIM);
                    return;
                    
                case DRAW_STIM:
                    rect.setSize(Vector2f(width * .1, width * .1));
                    center(rect);
                    rect.setFillColor(Color(intensity, intensity, intensity));
                    clear(bg);
                    draw(rect);
                    display();
                    set_state(STIM);
                    return;
                    
                case STIM:
                    rt = keyp(KEYSPACE) - state_start();
                    if(rt > 0){
                        set_trial_data("rt", (int)rt);
                        set_trial_data("intensity", intensity);
                        set_trial_data("delay", delay);
                        if(cnd("stage") != "test"){
                            set_state(FEEDBACK);
                        }else{
                            set_state(FINAL_CLEAR);
                        }
                    }
                    return;
                                        
                case FEEDBACK:
                    if(keyp(KEYSPACE) <= state_start()){
                        clear(bg);
                        draw_text("Twój czas reakcji w milisekundach: " + to_string(rt));
                        draw_text("Naciśnij spację", .5, .7);
                        display();
                    }else{
                        set_state(FINAL_CLEAR);
                    }
                    return;

                case FINAL_CLEAR:
                    clear(bg);
                    display();
                    TRIAL_IS_OVER = true;
                    return;
            }
        }
};

int main(){
    
    set_project_name("srt");
    get_user_data(user_data_instr);
    
    SRT task;
    
    unique_ptr<Instruction>(new Instruction(string_from_file("./instrukcja.utxt"), {"Dalej"}));
    
    task.init("test", {{"stage", {"practice"}}}, 5);
    task.run();
    
    string choice;
    while(choice != "Dalej"){
        unique_ptr<Instruction> instr = unique_ptr<Instruction>(new Instruction("Jeżeli nie masz wątpliwości na temat przebiegu zadania, naciśnij przycisk 'Dalej'.\n\n"
                "W przeciwnym razie możesz powtórzyć próbę treningową.", {"Dalej", "Powtórz"}));
                choice = instr->value;
                if(choice == "Powtórz")
                    task.init("test", {{"stage", {"practice"}}}, 5);
                    task.run();
    }
    
    unique_ptr<Instruction>(new Instruction("Teraz rozpocznie się właściwy etap zadania. Ten etap potrwa dłużej, niż etap treningowy i nie będą już pokazywane informacje o Twoim czasie reakcji.\n\n"
            "Naciśnij przycisk 'Dalej', aby kontynuować."));
    
    task.init("test", {{"stage", {"test"}}}, 100);
    task.run();
    
    return 0;
}
