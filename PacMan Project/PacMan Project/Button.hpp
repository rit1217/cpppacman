

#ifndef Button_hpp
#define Button_hpp

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

enum button_states{ PRESSED = 0, HOVER, IDLE };

class Button {
private:
    button_states buttonState;
    bool press, hover;
    
    RectangleShape outter, inner;
    Font* font;
    Text text;
    
    Color idleColor;
    Color hoverColor;
    Color textColor;
public:
    Button( float x, float y, float w, float h, string t, Font* f,
           Color idle, Color hover,Color fontColor, int s );
    void update( const Vector2f& mousePos );
    void render( RenderTarget* target );    
    const bool isPressed() const {
        if ( buttonState == PRESSED )
            return true;
        return false;
    }
};

#endif
