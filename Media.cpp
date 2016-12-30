#include "Media.hpp"

void Media::init(){
  key_pressed.resize(Keyboard::Key::KeyCount);
  key_released.resize(Keyboard::Key::KeyCount);
  mouse_pressed.resize(Mouse::Button::ButtonCount);
  mouse_released.resize(Mouse::Button::ButtonCount);

  cout << "Tworzê obiekt RenderWindow." << endl;
  window = new RenderWindow(VideoMode().getDesktopMode(), "Task", Style::Fullscreen);
  window->setVerticalSyncEnabled(true);
  window->setMouseCursorVisible(false);

  string font_name = "/usr/share/fonts/truetype/lato/Lato-Regular.ttf";
  if(!font.loadFromFile(font_name))
    throw(runtime_error("Nie uda³o siê za³adowaæ domy¶lnej czcionki: " + font_name));
  text.setFont(font);
  width = VideoMode().getDesktopMode().width; 
  height = VideoMode().getDesktopMode().height;
  text.setCharacterSize((float)height * 0.03);
}

void Media::close(){
  if(window->isOpen())
    window->close();
  delete window;
}

sf::String Media::utf32(string s){
  return String::fromUtf8(s.begin(), s.end());
}

void Media::process_events(Event &event){
  while(window->pollEvent(event)){
    switch(event.type){
    case Event::KeyPressed :
      key_pressed[event.key.code] = task_time();
      if(event.key.code == Keyboard::Escape)
        window->close();
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
