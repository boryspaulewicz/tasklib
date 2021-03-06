#ifndef MEDIA
#define MEDIA

#include "Common.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include "States.hpp"
#include "PType.hpp"
using namespace std;
using namespace sf;

enum class Alignment {
    LEFT, RIGHT, TOP, BOTTOM, CENTER
};

class Media : public States {
private:

    static unique_ptr<RenderWindow> win;
    static time_type some_key_pressed;
    static vector<time_type> key_pressed, key_released, mouse_pressed, mouse_released;
    static int mouse_x_pos, mouse_y_pos;

public:

    static void set_active() {
        win->setActive();
    }

    static void set_mouse_visible(bool value){
        win->setMouseCursorVisible(value);
    }

    static void display() {
        win->display();
    }

    static void clear(const Color& color = Color(0, 0, 0, 255)) {
        win->clear(color);
    }

    static void draw(const Drawable &drawable) {
        win->draw(drawable);
    }

    static Color bg, fg;

    static Event event;
    static RectangleShape rect;
    static Font font;
    static Text text;

    static int width, height;

    static void init();
    static void close();

    static bool poll_event(){ return win->pollEvent(event); }
    static void process_event();
    static void process_all_events(){ while(poll_event())process_event(); }
    
    static time_type some_keyp() {
        return some_key_pressed;
    }

    static time_type keyp(int key) {
        return key_pressed[key];
    }

    static time_type keyr(int key) {
        return key_released[key];
    }

    static time_type mousep(int button) {
        return mouse_pressed[button];
    }

    static time_type mouser(int button) {
        return mouse_released[button];
    }

    static int mouse_x() {
        return mouse_x_pos;
    }

    static int mouse_y() {
        return mouse_y_pos;
    }

    template<class T>
    static void center(T&obj);
    static void center_position(Transformable& obj, float x_offset = 0, float y_offset = 0);
    template<class T>
    static void align_origin(T& obj, Alignment horizontal = Alignment::CENTER, Alignment vertical = Alignment::CENTER);
    static sf::String utf32(string s);

};

/**
 * Ustala położenie origin na środek (obrysu) obiektu i przesuwa origin na
 * środek ekranu
 * @param obj
 */
template<class T>
inline void Media::center(T& obj) {
    Media::align_origin(obj);
    Media::center_position(obj);
}

/**
 * Ustala położenie origin obiektu graficznego na środek/względem środka ekranu
 * @param obj Obiekt dziedziczący z Transformable
 * @param x_offset poziome przesunięcie origin względem środka w proporcjach ekranu
 * @param y_offset pionowe przesunięcie origin względem środka w proporcjach ekranu
 */
inline void Media::center_position(Transformable& obj, float x_offset, float y_offset) {
    obj.setPosition(Vector2f(Media::width / 2 + x_offset * Media::width, Media::height / 2 + y_offset * Media::height));
}

/**
 * Ustala punkt origin względem prostokątu ograniczającego obiekt graficzny.
 * Patrz enum class Alignment.
 * @param obj Obiekt implementujący metody setOrigin i getLocalBounds
 * @param horizontal położenie origin w poziomie
 * @param vertical położenie origin w pionie
 */
template<class T>
inline void Media::align_origin(T& obj, Alignment hor, Alignment vert) {
    auto bounds = obj.getLocalBounds();
    float x, y;
    switch (hor) {
        case Alignment::CENTER:
            x = bounds.left + bounds.width / 2;
            break;
        case Alignment::LEFT:
            x = 0;
            break;
        case Alignment::RIGHT:
            x = bounds.left + bounds.width;
            break;
        default:
            throw (runtime_error("Błędna wartość wyrównania horyzontalnego"));
            break;
    }
    switch (vert) {
        case Alignment::CENTER:
            y = bounds.top + bounds.height / 2;
            break;
        case Alignment::TOP:
            y = 0;
            break;
        case Alignment::BOTTOM:
            y = bounds.top + bounds.height;
            break;
        default:
            throw (runtime_error("Błędna wartość wyrównania wertykalnego"));
            break;
    }
    obj.setOrigin(x, y);
}

enum Key {
    KEYUNKNOWN = -1, ///< Unhandled key
    KEYA = 0, ///< The A key
    KEYB, ///< The B key
    KEYC, ///< The C key
    KEYD, ///< The D key
    KEYE, ///< The E key
    KEYF, ///< The F key
    KEYG, ///< The G key
    KEYH, ///< The H key
    KEYI, ///< The I key
    KEYJ, ///< The J key
    KEYK, ///< The K key
    KEYL, ///< The L key
    KEYM, ///< The M key
    KEYN, ///< The N key
    KEYO, ///< The O key
    KEYP, ///< The P key
    KEYQ, ///< The Q key
    KEYR, ///< The R key
    KEYS, ///< The S key
    KEYT, ///< The T key
    KEYU, ///< The U key
    KEYV, ///< The V key
    KEYW, ///< The W key
    KEYX, ///< The X key
    KEYY, ///< The Y key
    KEYZ, ///< The Z key
    KEYNUM0, ///< The 0 key
    KEYNUM1, ///< The 1 key
    KEYNUM2, ///< The 2 key
    KEYNUM3, ///< The 3 key
    KEYNUM4, ///< The 4 key
    KEYNUM5, ///< The 5 key
    KEYNUM6, ///< The 6 key
    KEYNUM7, ///< The 7 key
    KEYNUM8, ///< The 8 key
    KEYNUM9, ///< The 9 key
    KEYESCAPE, ///< The Escape key
    KEYLCONTROL, ///< The left Control key
    KEYLSHIFT, ///< The left Shift key
    KEYLALT, ///< The left Alt key
    KEYLSYSTEM, ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
    KEYRCONTROL, ///< The right Control key
    KEYRSHIFT, ///< The right Shift key
    KEYRALT, ///< The right Alt key
    KEYRSYSTEM, ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
    KEYMENU, ///< The Menu key
    KEYLBRACKET, ///< The [ key
    KEYRBRACKET, ///< The ] key
    KEYSEMICOLON, ///< The ; key
    KEYCOMMA, ///< The , key
    KEYPERIOD, ///< The . key
    KEYQUOTE, ///< The ' key
    KEYSLASH, ///< The / key
    KEYBACKSLASH, ///< The \ key
    KEYTILDE, ///< The ~ key
    KEYEQUAL, ///< The = key
    KEYDASH, ///< The - key
    KEYSPACE, ///< The Space key
    KEYRETURN, ///< The Return key
    KEYBACKSPACE, ///< The Backspace key
    KEYTAB, ///< The Tabulation key
    KEYPAGEUP, ///< The Page up key
    KEYPAGEDOWN, ///< The Page down key
    KEYEND, ///< The End key
    KEYHOME, ///< The Home key
    KEYINSERT, ///< The Insert key
    KEYDELETE, ///< The Delete key
    KEYADD, ///< The + key
    KEYSUBTRACT, ///< The - key
    KEYMULTIPLY, ///< The * key
    KEYDIVIDE, ///< The / key
    KEYLEFT, ///< Left arrow
    KEYRIGHT, ///< Right arrow
    KEYUP, ///< Up arrow
    KEYDOWN, ///< Down arrow
    KEYNUMPAD0, ///< The numpad 0 key
    KEYNUMPAD1, ///< The numpad 1 key
    KEYNUMPAD2, ///< The numpad 2 key
    KEYNUMPAD3, ///< The numpad 3 key
    KEYNUMPAD4, ///< The numpad 4 key
    KEYNUMPAD5, ///< The numpad 5 key
    KEYNUMPAD6, ///< The numpad 6 key
    KEYNUMPAD7, ///< The numpad 7 key
    KEYNUMPAD8, ///< The numpad 8 key
    KEYNUMPAD9, ///< The numpad 9 key
    KEYF1, ///< The F1 key
    KEYF2, ///< The F2 key
    KEYF3, ///< The F3 key
    KEYF4, ///< The F4 key
    KEYF5, ///< The F5 key
    KEYF6, ///< The F6 key
    KEYF7, ///< The F7 key
    KEYF8, ///< The F8 key
    KEYF9, ///< The F9 key
    KEYF10, ///< The F10 key
    KEYF11, ///< The F11 key
    KEYF12, ///< The F12 key
    KEYF13, ///< The F13 key
    KEYF14, ///< The F14 key
    KEYF15, ///< The F15 key
    KEYPAUSE, ///< The Pause key
    KEYKEYCOUNT ///< Keep last -- the total number of keyboard keys
};

#endif
