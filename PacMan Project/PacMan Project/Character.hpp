
#ifndef Character_hpp
#define Character_hpp
#include "Constant.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct sprite_path
{
    vector<string> left, right, up, down;
};

class Character
{
public:
    Character( Vector2i& start ) : start_pos(start) {}
    virtual void draw( RenderWindow& ) = 0;
    virtual bool movable() = 0;
    virtual void load_elements() = 0;
    
    Vector2i get_direction() const {
        return direction;
    }
    Vector2f get_pic_pos() const {
        return pic_pos;
    }
    Vector2i get_grid_pos() const {
        return grid_pos;
    }
protected:
    Vector2i grid_pos, start_pos, direction;
    Vector2f pic_pos;
    int speed, move_count;
    Sprite sprite;
    Texture texture;
    struct sprite_path sprite_path_list;
};


#endif
