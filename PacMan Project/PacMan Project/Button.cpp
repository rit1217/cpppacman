
#include "Button.hpp"

Button::Button( float x, float y, float w, float h, string t, Font* f, Color idle, Color hover, Color fontColor, int s )
{
    outter.setSize( Vector2f( w, h ) );
    outter.setPosition( Vector2f( x, y) );
    inner.setSize( Vector2f( w - h * 0.1, h * 0.9 ) );
    inner.setPosition( Vector2f( x + h * 0.05, y + h * 0.05));
    font = f;
    text.setFont( *font );
    text.setString( t );
    text.setFillColor( fontColor );
    text.setCharacterSize( s );
    text.setPosition(
                     outter.getPosition().x + outter.getGlobalBounds().width / 2.f - text.getGlobalBounds().width / 2.f
                     ,  outter.getPosition().y + outter.getGlobalBounds().height / 2.f - text.getGlobalBounds().height / 2.f - 10 );
    
    idleColor = idle;
    hoverColor = hover;
    textColor = fontColor;
    buttonState = IDLE;
    outter.setFillColor( idle );
}

void Button::render( RenderTarget* target )
{
    target->draw( outter );
    target->draw( inner );
    target->draw( text );
}

void Button::update( const Vector2f& mousePos )
{
    if ( outter.getGlobalBounds().contains( mousePos ))
    {
        buttonState = HOVER;
        if ( Mouse::isButtonPressed( Mouse::Left ) )
        {
            buttonState = PRESSED;
        }
    }
    else buttonState = IDLE;
    
    switch ( buttonState )
    {
        case IDLE:
            outter.setFillColor( idleColor );
            inner.setFillColor( Color( 41, 37, 39) );
            text.setFillColor( textColor );
            break;
        case HOVER:
            outter.setFillColor( Color::White );
            inner.setFillColor( hoverColor );
            text.setFillColor( Color( 41, 37, 39 ) );
            break;
        case PRESSED:
            outter.setFillColor( hoverColor );
            inner.setFillColor( Color::White );
            text.setFillColor( Color( 41, 37, 39 ) );
            break;
    }

}

