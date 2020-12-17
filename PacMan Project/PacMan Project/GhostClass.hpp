
#ifndef GhostClass_hpp
#define GhostClass_hpp
#include <SFML/Graphics.hpp>
#include "Constant.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "Character.hpp"

using namespace std;
using namespace sf;

class Ghost : public Character{
private:
    vector<Vector2<int>> walls, decision_points, house_entrance;

    virtual void find_target( Vector2<int> pac_pos, Vector2<int> pac_direction, Vector2<int> blinky_pos ) = 0;
    
    bool movable();
    Vector2<int> find_turn();
    void make_decision();
public:
    Ghost ( Vector2<int> start, vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e );
    Time start_mode;
    
    Vector2f find_pic_pos() const {
        return Vector2f( grid_pos.x * GRID_SIZE - 10, grid_pos.y * GRID_SIZE + 85 );
    }
    
    void set_pic_pos( Vector2f newP ) { pic_pos = newP; }
    
    string get_mode() const { return mode; }

    void update( Vector2<int> pac_pos, Vector2<int> pac_direction, Vector2<int> blinky_pos );
    void change_mode ( string newM );
    void draw( RenderWindow& ) ;
    
    virtual void pac_revive();
    virtual void restart ( vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e );
    virtual void load_elements() = 0;

protected:
    string mode;
    Vector2<int> last_dec_point, target;
    bool in_house;
    vector<string> frighten_sprite_path;
    
    double find_distance( Vector2<int> start, Vector2<int> end );
};

class Blinky : public Ghost {
private:
    void find_target( Vector2<int> pac_pos, Vector2<int> pac_direction, Vector2<int> blinky_pos );
public:
    Blinky( Vector2<int> start, vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e );
    void restart ( vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e );
    void pac_revive();
    void load_elements();
};

class Pinky : public Ghost {
private:
    void find_target( Vector2<int> pac_pos, Vector2<int> pac_direction, Vector2<int> blinky_pos );
public:
    Pinky( Vector2<int> start, vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e ) :
    Ghost( start, w, d, e ) {}
    void load_elements();
};

class Inky : public Ghost {
private:
    void find_target( Vector2<int> pac_pos, Vector2<int> pac_direction, Vector2<int> blinky_pos );
public:
    Inky( Vector2<int> start, vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e ) :
    Ghost( start, w, d, e ) {}
    void load_elements();
};

class Clyde : public Ghost {
private:
    void find_target( Vector2<int> pac_pos, Vector2<int> pac_direction, Vector2<int> blinky_pos );
public:
    Clyde( Vector2<int> start, vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e ) :
    Ghost( start, w, d, e ) {}
    void load_elements();

};


#endif
