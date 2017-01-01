#include "Media.hpp"

void Media::white_on_black(){
  bg = Color(Color::Black);
  fg = Color(Color::White);
}

void Media::init(){
  create(VideoMode().getDesktopMode(), "Task", Style::Fullscreen);

  setVerticalSyncEnabled(true);
  setMouseCursorVisible(false);

  // Inaczej zostaj± dane z poprzedniego wykonania pêtli prób
  key_pressed.clear();
  key_released.clear();
  mouse_pressed.clear();
  mouse_released.clear();
  key_pressed.resize(Keyboard::Key::KeyCount);
  key_released.resize(Keyboard::Key::KeyCount);
  mouse_pressed.resize(Mouse::Button::ButtonCount);
  mouse_released.resize(Mouse::Button::ButtonCount);
  
  string font_name = "/usr/share/fonts/truetype/lato/Lato-Regular.ttf";
  if(!font.loadFromFile(font_name))
    throw(runtime_error("Nie uda³o siê za³adowaæ domy¶lnej czcionki: " + font_name));
  text.setFont(font);
  width = VideoMode().getDesktopMode().width; 
  height = VideoMode().getDesktopMode().height;
  text.setCharacterSize((float)height * 0.03);
}

sf::String Media::utf32(string s){
  return String::fromUtf8(s.begin(), s.end());
}

void Media::process_events(Event &event){
  while(pollEvent(event)){
    switch(event.type){
    case Event::KeyPressed :
      key_pressed[event.key.code] = task_time();
      if(event.key.code == Keyboard::Escape)
        close();
      break;
    case Event::KeyReleased :
      key_released[event.key.code] = task_time();
      break;
    case Event::MouseButtonPressed :
      mouse_pressed[event.mouseButton.button] = task_time();
      break;
    case Event::MouseButtonReleased :
      mouse_released[event.mouseButton.button] = task_time();
      break;
    default:
      break;
    }
  }
}
