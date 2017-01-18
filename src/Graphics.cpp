#include <SFML/Graphics/Image.hpp>

#include "Graphics.hpp"

void draw_gabor(sf::Image& im, float f = 20, float deg_angle = 45, 
        float sigma = .01, float contrast = .3){
    im.create(Media::width * 9 * sigma, Media::height * 9 * sigma, Color::Black);
    float width = im.getSize().x;
    float height = im.getSize().y;
    sigma = sigma * Media::width;
    // radiany
    float angle = deg_angle * M_PI / 180.0;
    float cos_angle = cos(angle);
    float sin_angle = sin(angle);
    float two_sqr_sigma = 2 * pow(sigma, 2);
    float x_middle = width / 2;
    float y_middle = height / 2;
    float two_pi_by_height = 2 * M_PI / (float)Media::height;
    // Jasność określona przez odległość (kwadrat dla oszczędzenia liczenia) od środka ekranu
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++) {
            // Widoczność proporcjonalna do dnorm(punkt - środek) = 
            // c * exp(-(x - mu)^2 / (2 * sigma^2))
            float visibility = exp(-((pow(x - x_middle, 2) + pow(y - y_middle, 2)) / two_sqr_sigma));
            // Dla distance = 0 mamy visibility == 1
            float scaling = 0.5 * contrast * visibility;
            // Liczymy i rysujemy nie-szarość tylko, gry w tym punkcie 
            // widoczność jest niepomijalna. .002 * 255 = .450 
            if(scaling > 0.002){
                // Obracamy układ współrzędnych
                float new_y = y * cos_angle + x * sin_angle;
                // Przekształcamy ze skali -.5 -  +.5 do 0 - 255
                float v = 255.0 / 2.0 * (1 + scaling * sin(new_y * two_pi_by_height * f));
                im.setPixel(x, y, Color(v, v, v));
            }else{
                im.setPixel(x, y, Color(.5, .5, .5));
            }
        }
    }
}
