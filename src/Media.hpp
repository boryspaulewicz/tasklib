#ifndef MEDIA
#define MEDIA

#include<vector>
#include<iostream>
#include<chrono>
#include<SFML/Graphics.hpp>
using namespace std;
using namespace sf;
using namespace chrono;

using ms = long int;

class Media{

private:

  vector<ms> key_pressed, key_released, mouse_pressed, mouse_released;
  
public:

  void init();

  void close();

  RenderWindow* window;
  Event event;
  int width, height;
  RectangleShape rect;
  Font font;
  Text text;

  void process_events(Event &event);

  ms keyp(int key){
    return key_pressed[key];
  }

  ms keyr(int key){
    return key_released[key];
  }

  ms mousep(int button){
    return mouse_pressed[button];
  }

  ms mouser(int button){
    return mouse_released[button];
  }
  
  template<class T>
  void center(T& obj);

  sf::String utf32(string s);

  int state;
  using tp = high_resolution_clock::time_point;
  tp state_start, trial_start, task_start;

  inline void set_state(int s){
    state = s;
    state_start = high_resolution_clock::now();
  }

  inline ms state_time(){
    return duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() -
                                                    state_start).count();  
  }
  
  inline ms trial_time(){
    return duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() -
                                                    trial_start).count();
  }
  
  inline ms task_time(){
    return duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() -
                                                    task_start).count();
  }

};

template<class T>
void Media::center(T& obj){
  auto bounds = obj.getLocalBounds();
  obj.setOrigin(bounds.width / 2, bounds.height / 2);
  obj.setPosition(width / 2, height / 2);
}

enum Key{
  KEYUNKNOWN = -1, ///< Unhandled key
  KEYA = 0,        ///< The A key
  KEYB,            ///< The B key
  KEYC,            ///< The C key
  KEYD,            ///< The D key
  KEYE,            ///< The E key
  KEYF,            ///< The F key
  KEYG,            ///< The G key
  KEYH,            ///< The H key
  KEYI,            ///< The I key
  KEYJ,            ///< The J key
  KEYK,            ///< The K key
  KEYL,            ///< The L key
  KEYM,            ///< The M key
  KEYN,            ///< The N key
  KEYO,            ///< The O key
  KEYP,            ///< The P key
  KEYQ,            ///< The Q key
  KEYR,            ///< The R key
  KEYS,            ///< The S key
  KEYT,            ///< The T key
  KEYU,            ///< The U key
  KEYV,            ///< The V key
  KEYW,            ///< The W key
  KEYX,            ///< The X key
  KEYY,            ///< The Y key
  KEYZ,            ///< The Z key
  KEYNUM0,         ///< The 0 key
  KEYNUM1,         ///< The 1 key
  KEYNUM2,         ///< The 2 key
  KEYNUM3,         ///< The 3 key
  KEYNUM4,         ///< The 4 key
  KEYNUM5,         ///< The 5 key
  KEYNUM6,         ///< The 6 key
  KEYNUM7,         ///< The 7 key
  KEYNUM8,         ///< The 8 key
  KEYNUM9,         ///< The 9 key
  KEYESCAPE,       ///< The Escape key
  KEYLCONTROL,     ///< The left Control key
  KEYLSHIFT,       ///< The left Shift key
  KEYLALT,         ///< The left Alt key
  KEYLSYSTEM,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
  KEYRCONTROL,     ///< The right Control key
  KEYRSHIFT,       ///< The right Shift key
  KEYRALT,         ///< The right Alt key
  KEYRSYSTEM,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
  KEYMENU,         ///< The Menu key
  KEYLBRACKET,     ///< The [ key
  KEYRBRACKET,     ///< The ] key
  KEYSEMICOLON,    ///< The ; key
  KEYCOMMA,        ///< The , key
  KEYPERIOD,       ///< The . key
  KEYQUOTE,        ///< The ' key
  KEYSLASH,        ///< The / key
  KEYBACKSLASH,    ///< The \ key
  KEYTILDE,        ///< The ~ key
  KEYEQUAL,        ///< The = key
  KEYDASH,         ///< The - key
  KEYSPACE,        ///< The Space key
  KEYRETURN,       ///< The Return key
  KEYBACKSPACE,    ///< The Backspace key
  KEYTAB,          ///< The Tabulation key
  KEYPAGEUP,       ///< The Page up key
  KEYPAGEDOWN,     ///< The Page down key
  KEYEND,          ///< The End key
  KEYHOME,         ///< The Home key
  KEYINSERT,       ///< The Insert key
  KEYDELETE,       ///< The Delete key
  KEYADD,          ///< The + key
  KEYSUBTRACT,     ///< The - key
  KEYMULTIPLY,     ///< The * key
  KEYDIVIDE,       ///< The / key
  KEYLEFT,         ///< Left arrow
  KEYRIGHT,        ///< Right arrow
  KEYUP,           ///< Up arrow
  KEYDOWN,         ///< Down arrow
  KEYNUMPAD0,      ///< The numpad 0 key
  KEYNUMPAD1,      ///< The numpad 1 key
  KEYNUMPAD2,      ///< The numpad 2 key
  KEYNUMPAD3,      ///< The numpad 3 key
  KEYNUMPAD4,      ///< The numpad 4 key
  KEYNUMPAD5,      ///< The numpad 5 key
  KEYNUMPAD6,      ///< The numpad 6 key
  KEYNUMPAD7,      ///< The numpad 7 key
  KEYNUMPAD8,      ///< The numpad 8 key
  KEYNUMPAD9,      ///< The numpad 9 key
  KEYF1,           ///< The F1 key
  KEYF2,           ///< The F2 key
  KEYF3,           ///< The F3 key
  KEYF4,           ///< The F4 key
  KEYF5,           ///< The F5 key
  KEYF6,           ///< The F6 key
  KEYF7,           ///< The F7 key
  KEYF8,           ///< The F8 key
  KEYF9,           ///< The F9 key
  KEYF10,          ///< The F10 key
  KEYF11,          ///< The F11 key
  KEYF12,          ///< The F12 key
  KEYF13,          ///< The F13 key
  KEYF14,          ///< The F14 key
  KEYF15,          ///< The F15 key
  KEYPAUSE,        ///< The Pause key
  KEYKEYCOUNT      ///< Keep last -- the total number of keyboard keys
};

#endif
