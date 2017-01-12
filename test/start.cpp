// -*- coding: utf-8 -*-

#include "Task.hpp"

#define FIXATION_TIME 1000
#define PRE_FIXATION_TIME 500

class Test : public Task{
public:
  
  RectangleShape fixation;

  void draw_fixation(){
    fixation.setSize(Vector2f((float)width * .05, 1));
    center(fixation);
    fixation.setRotation(45);
    draw(fixation);
    fixation.setRotation(-45);
    draw(fixation);
  }

  void draw_text(string s, float x = .5, float y = .5){
    text.setString(utf32(s));
    center(text);
    text.setPosition(Vector2f(width * x, height * y));
    draw(text);
  }
  
  long int stim_start, space_start, rt, feedback_start;
  int intensity, delay;
  
  enum stage{START, DRAW_WFS, WFS, PRE_FIXATION, DRAW_FIXATION, FIXATION, CLEAR_FIXATION, BLANK, DRAW_STIMULUS, STIMULUS, FEEDBACK};
  
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
          set_state(DRAW_WFS);
        }else{
          set_state(DRAW_FIXATION);
        }
      return;
      
    case DRAW_WFS:
      clear(bg);
      draw_text("Naciśnij spację, aby rozpocząć zadanie");
      display();
      space_start = time_ms();
      set_state(WFS);
      return;
        
    case WFS:
      if(keyp(KEYSPACE) > space_start){
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
        set_state(DRAW_STIMULUS);
      return;

    case DRAW_STIMULUS:
      rect.setSize(Vector2f(width * .1, width * .1));
      center(rect);
      rect.setFillColor(Color(intensity, intensity, intensity));
      clear(bg);
      draw(rect);
      display();
      stim_start = time_ms();
      set_state(STIMULUS);
      return;
        
    case STIMULUS:
      rt = keyp(KEYSPACE) - stim_start;
      if(rt > 0){
        clear(bg);
        display();
        set_trial_data("RT", (int)rt);
        set_trial_data("intensity", intensity);
        set_trial_data("delay", delay);
        if(cnd("stage") == "practice"){
          feedback_start = time_ms();
          set_state(FEEDBACK);
        }else{
          TRIAL_IS_OVER = true;
          return;
        }
      }
      return;
      
    case FEEDBACK:
      if(keyp(KEYSPACE) <= feedback_start){
        clear(bg);
        draw_text("Twój czas reakcji w milisekundach: " + to_string(rt));
        draw_text("Naciśnij spację", .5, .7);
        display();
      }else{
        clear(bg);
        display();
        TRIAL_IS_OVER = true;
      }
      return;
    }
  }
};

int main(){

  get_user_data(user_data_instr);

  Test task;

  unique_ptr<Instruction> instr(new Instruction(load_from_file("./instrukcja.utxt"), {"Dalej"}));
  
  task.init("test", {{"stage", {"practice"}}}, 5);
  task.run();

  string choice;
  while(choice != "Dalej"){
    instr = unique_ptr<Instruction>(new Instruction("Jeżeli nie masz wątpliwości na temat przebiegu zadania, naciśnij przycisk 'Dalej'.\n\n"
                                                    "W przeciwnym razie możesz powtórzyć próbę treningową.", {"Dalej", "Powtórz"}));
    choice = instr->value;
    if(choice == "Powtórz")
      task.init("test", {{"stage", {"practice"}}}, 5);
      task.run();
  }

  instr = unique_ptr<Instruction>(new Instruction("Teraz rozpocznie się właściwy etap zadania. Ten etap potrwa dłużej, niż etap treningowy i nie będą już pokazywane informacje o Twoim czasie reakcji.\n\n"
                                                  "Naciśnij przycisk 'Dalej', aby kontynuować."));
  
  task.init("test", {{"stage", {"test"}}}, 100);
  task.run();

  return 0;
}
