// -*- coding: utf-8 -*-

#include "Media.hpp"

unique_ptr<RenderWindow> Media::win;
Event Media::event;

time_type Media::some_key_pressed;
vector<time_type> Media::key_pressed;
vector<time_type> Media::key_released;
vector<time_type> Media::mouse_pressed;
vector<time_type> Media::mouse_released;
int Media::mouse_x_pos;
int Media::mouse_y_pos;

int Media::width;
int Media::height;
Color Media::fg;
Color Media::bg;
Font Media::font;
Text Media::text;
RectangleShape Media::rect;

void Media::init(){
    
    win = unique_ptr<RenderWindow>(new RenderWindow(VideoMode().getDesktopMode(), "Task", Style::Fullscreen));
    win->setVerticalSyncEnabled(true);
    win->setMouseCursorVisible(false);
    
    width = VideoMode().getDesktopMode().width; 
    height = VideoMode().getDesktopMode().height;
    fg = Color::White;
    bg = Color::Black;
    
    // Inaczej zostają dane z poprzedniego wykonania pętli prób
    some_key_pressed = 0;
    key_pressed.clear();
    key_released.clear();
    mouse_pressed.clear();
    mouse_released.clear();
    key_pressed.resize(Keyboard::Key::KeyCount);
    key_released.resize(Keyboard::Key::KeyCount);
    mouse_pressed.resize(Mouse::Button::ButtonCount);
    mouse_released.resize(Mouse::Button::ButtonCount);
    
    // string font_name = "/usr/share/fonts/truetype/freefont/FreeMono.ttf";
    string font_name = "/usr/share/fonts/truetype/lato/Lato-Regular.ttf";
    if(!font.loadFromFile(font_name))
        throw(runtime_error("Nie udało się załadować domyślnej czcionki: " + font_name));
    text.setFont(font);
    text.setCharacterSize((float)height * 0.05);
    
    // To wydaje się eliminować błąd "Failed to open BO for returned
    // DRI2 buffer"
    // while(win->pollEvent(event)){}
    // win->setActive(); // to nie wystarcza
    time_type small_break = now_ms();
    auto cfg = read_cfg("cfg.txt", false);
    while((now_ms() - small_break) < (cfg.count("media_delay_ms") == 1 ? (int)cfg["media_delay_ms"] : 200)){}
}

void Media::close(){
    // win->close(); // razem z = nullptr powoduje naruszenie ochrony pamięci w nowej SFML
    win = nullptr;
}

/**
 * Funkcja pomocnicza, zamieniająca string w utf8 na utf32 używany przez SFML
 * @param s
 * @return 
 */
sf::String Media::utf32(string s){
    return String::fromUtf8(s.begin(), s.end());
}

/**
 * Uaktualnia informacje o momencie wystąpienia ewentualnych zdarzeń wejścia,
 * takich jak naciśnięcie/zwolnienie klawisza, przycisku myszki, czy przesunięcie
 * kursora.
 */
void Media::process_inputs(){
    while(win->pollEvent(event)){
        switch(event.type){
            case Event::KeyPressed :
                some_key_pressed = now_ms();
                key_pressed[event.key.code] = some_key_pressed;
                break;
            case Event::KeyReleased :
                key_released[event.key.code] = now_ms();
                break;
            case Event::MouseButtonPressed :
                mouse_pressed[event.mouseButton.button] = now_ms();
                break;
            case Event::MouseButtonReleased :
                mouse_released[event.mouseButton.button] = now_ms();
                break;
            case Event::MouseMoved :
                mouse_x_pos = event.mouseMove.x;
                mouse_y_pos = event.mouseMove.y;
                break;
            default:
                break;
        }
    }
}
