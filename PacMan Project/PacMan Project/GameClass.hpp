
#ifndef GameClass_hpp
#define GameClass_hpp

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include "Constant.hpp"
#include "PacClass.hpp"
#include "GhostClass.hpp"
#include "Button.hpp"

using namespace std;
using namespace sf;

struct start_position
{
    Vector2<int> pac, blinky, pinky, inky, clyde;
};

class Game {
private:
    vector<Vector2<int>> walls, dots, energ, ghost_en, ghost_dec;
    struct start_position start_pos;
    int point, win, play;
    bool reviving;
    CircleShape dot, energizer_dot;
    vector<Ghost*> ghosts;
    
    Texture maze_texture , menu_texture, win_texture, lose_texture;
    Sprite background, maze;
    
    Time game_time, start_die;
    string scene;
    Font pac_font, arcade_font;
    SoundBuffer eat_dot_sound, eat_en_sound,
                beginning_sound, death_sound, eat_ghost_sound;
    Sound sound;
    Text score_text, life_text, end_text, end_score_text;
    
    void load_elements();
    void draw_dots( RenderWindow& );
    void hit_ghost ( PacMan& );
    void restart_game( PacMan& );
    void prepare_game( RenderWindow&, int );
    void redraw_game( PacMan&, RenderWindow&, bool );
    void draw_ending( RenderWindow& );
    void draw_score_life( RenderWindow&, int );
public:
    Game();
    void run();
};

#endif
