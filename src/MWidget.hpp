#ifndef MWIDGET_HPP
#define MWIDGET_HPP

#include "Media.hpp"

class MWidget : public Text{
    private:
        bool active;
        
        bool is_inside(int x, int y){
            auto gb = getGlobalBounds();
            auto left = gb.left;
            auto right = gb.left + gb.width;
            auto top = gb.top;
            auto bottom = gb.top + gb.height;
            return ((x >= left) && (x <= right) && (y >= top) && (y <= bottom));
        }
        
        virtual void mouse_moved(int x, int y){ 
            if(active){
                if(is_inside(x, y)){
                    setFillColor(highlight_col);
                }else{
                    setFillColor(active_col);
                }
            }
        }
        
        virtual void mouse_pressed(int x, int y, int button){ 
            if(active && is_inside(x, y)){
                set_active(false);
                was_pressed = true;
                auto gb = getGlobalBounds();
                pressed_pos.x = (x - gb.left) / (float)gb.width;
                pressed_pos.y = (y - gb.top) / (float)gb.height;
            }
        }
        
        virtual void mouse_released(int x, int y, int button){ }
        
        public:
            
            bool was_pressed = false;
            Vector2f pressed_pos;
            
            Color highlight_col = Color::Green, 
                    inactive_col = Color(120, 120, 120), 
                    active_col = Color::White;

            MWidget(const string label, bool active = true) : active{active} 
            {
                setFont(Media::font);
                setString(label);
                auto lb = getLocalBounds();
                set_active(true);
            }
            
            virtual void set_active(bool value){
                active = value;
                if(active){
                    setFillColor(active_col);
                    was_pressed = false;
                }else{
                    setFillColor(inactive_col);
                }
            }
            
            void process_event(Event event){
                switch(event.type){
                    case Event::MouseMoved:
                        mouse_moved(event.mouseMove.x, event.mouseMove.y);                
                        break;
                    case Event::MouseButtonPressed:
                        mouse_pressed(event.mouseButton.x, event.mouseButton.y, 
                                event.mouseButton.button);
                        break;
                    case Event::MouseButtonReleased:
                        mouse_released(event.mouseButton.x, event.mouseButton.y, 
                                event.mouseButton.button);
                        break;
                    default:
                        break;
                }
            }
};

#endif /* WIDGET_HPP */

