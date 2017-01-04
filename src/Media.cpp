#include "Media.hpp"

void Media::white_on_black(){
  bg = Color(Color::Black);
  fg = Color(Color::White);
}

void Media::init(){

  win = unique_ptr<RenderWindow>(new RenderWindow(VideoMode().getDesktopMode(), "Task", Style::Fullscreen));
  win->setVerticalSyncEnabled(true);
  win->setMouseCursorVisible(false);
  win->clear();
  win->display();
  while(win->pollEvent(event)){}
  
  white_on_black();

  // Inaczej zostaj� dane z poprzedniego wykonania p�tli pr�b
  some_key_pressed = 0;
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
    throw(runtime_error("Nie uda�o si� za�adowa� domy�lnej czcionki: " + font_name));
  text.setFont(font);
  text.setCharacterSize((float)height * 0.03);

  state_durations.clear();
}

void Media::close(){
  win->close();
  win = nullptr;
}

Media::Media(){
  width = VideoMode().getDesktopMode().width; 
  height = VideoMode().getDesktopMode().height;
}

sf::String Media::utf32(string s){
  return String::fromUtf8(s.begin(), s.end());
}

void Media::process_events(Event &event){
  while(win->pollEvent(event)){
    switch(event.type){
    case Event::KeyPressed :
      some_key_pressed = task_time();
      key_pressed[event.key.code] = some_key_pressed;
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
