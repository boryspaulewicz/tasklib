#ifndef MWIDGET_HPP
#define MWIDGET_HPP

#include "Media.hpp"

enum class MouseButton {
    LBP, RBP, LBR, RBR, NONE
};

class MWidget : public RectangleShape {
private:
    bool active = false;
    decltype(now()) start;
    Text text;
    RectangleShape bar;
    Color highlight = Color::Green, inactive = Color::Blue;

    bool mouse_inside(int x, int y) {
        return false;
    }

public:

    MWidget(const string label) {
        text.setFont(Media::font);
        text.setString(label);
        setFillColor(Color::Black);
        setOutlineColor(Color::White);
        setOutlineThickness(1);
        text.setFillColor(Color::White);
        text.setOutlineColor(Color::Black);
    }

    void set_active() {
        if (!active) {
            active = true;
            start = now();
        }
    }

    void set_inactive() {
        active = false;
    }
};

#endif /* WIDGET_HPP */

