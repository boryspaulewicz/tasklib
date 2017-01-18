#ifndef MWIDGET_HPP
#define MWIDGET_HPP

#include "Media.hpp"

class MWidget : public RectangleShape {
private:
    bool active = false;
    Text text;
    RectangleShape bar;
    Color highlight = Color::Green, inactive = Color::Blue;

    void mouse_move(int x, int y){ }
    void mouse_clicked(int x, int y, int button){ }
    void mouse_released(int x, int y, int button){ }

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

    void set_active() { if (!active)active = true; }
    void set_inactive() { active = false; }
};

#endif /* WIDGET_HPP */

