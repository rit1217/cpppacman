
#ifndef PacClass_hpp
#define PacClass_hpp
#include <SFML/Graphics.hpp>
#include "Constant.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include "Character.hpp"

using namespace std;
using namespace sf;

class PacMan : public Character {
private:
    // dots, wall, ghost house entrances and energizers position
    vector<Vector2<int>>* dots;
    vector<Vector2<int>>* energ;
    vector<Vector2<int>> walls, ghost_en;
    double pac_radius;
    string status, state;
    
    bool eat_dots();
    bool eat_energizer();
    bool movable();
    vector<string> death_sprite_path;
    int death_frame;
    
public:
    PacMan( Vector2<int>& , vector<Vector2<int>>& , vector<Vector2<int>>& ,
    vector<Vector2<int>>& , vector<Vector2<int>>&  );
    
    int life;

    void set_status ( string  newS ) { status = newS; }

    void pac_move( Vector2<int>&  );
    bool turnable( Vector2<int>&  );
    string update( vector<Vector2<int>>&  );
    void restart( vector<Vector2<int>>& , vector<Vector2<int>>& ,
                 vector<Vector2<int>>& , vector<Vector2<int>>&  );
    void die();
    void revive();
    void load_elements();
    void draw( RenderWindow& ) ;
    void draw_die( RenderWindow& ) ;
};

#endif
