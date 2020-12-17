
#include "PacClass.hpp"
// constructor
// parameters : start position, wall position list, dots position list, energizer position list,
//              ghost house entrance position list
PacMan::PacMan( Vector2<int>& start, vector<Vector2<int>>& w, vector<Vector2<int>>& d,
                vector<Vector2<int>>& e, vector<Vector2<int>>& gh ) : Character( start )
{
    start_pos = start;
    grid_pos = start;
    pac_radius = PAC_SIZE / 2;
    pic_pos.x = grid_pos.x * GRID_SIZE - (pac_radius - GRID_SIZE / 2);
    pic_pos.y = grid_pos.y * GRID_SIZE - (pac_radius - GRID_SIZE / 2) + 90;
    move_count = 0;
    direction = LEFT;
    status = "alive";
    state = "left";
    speed = CHARACTER_SPEED;
    walls = w;
    dots = &d;
    energ = &e;
    ghost_en = gh;
    life = 3;
    texture.loadFromFile( "PacSprite/pacman0.png" );
    sprite.setTexture( texture );
    sprite.setPosition( pic_pos );
    death_frame = 0;
}
// check whether Pacman can travel in the next directilon or not
// if next direction is not a walls or ghost house's entrance : this function will return true
bool PacMan::turnable( Vector2<int>& turn )
{
    bool res = true;
    Vector2<int> temp_direction = turn;
    double gridX, gridY;
    gridX = ( pic_pos.x + 10 ) / 40;
    gridY = ( pic_pos.y - 80 ) / 40;
    if ( fmod( gridX, 1 ) == 0 && fmod( gridY, 1 ) == 0 ) {
        for ( auto& i : walls )
            if ( Vector2<int>(static_cast<int>(gridX), static_cast<int>(gridY)) + temp_direction == i )
                res = false;
        for ( auto& i : ghost_en )
            if ( Vector2<int>(static_cast<int>(gridX), static_cast<int>(gridY)) + temp_direction == i )
            res = false;
    }
    else {
        res = false;
    }
    return res;
}
// move Pacman in the correct direction
void PacMan::pac_move( Vector2<int>& turn )
{
    if ( turnable(turn) ) {
        direction = turn;
        if ( turn == LEFT ) state = "left";
        else if ( turn == RIGHT ) state = "right";
        else if ( turn == UP )  state = "up";
        else if ( turn == DOWN ) state = "down";
    }
}
// update Pacman grid position, picture position
string PacMan::update( vector<Vector2<int>>& g_pos )
{
    string res = "";
    if ( status != "dying" ){
        if ( movable() ) {
            pic_pos.x += direction.x * speed;
            pic_pos.y += direction.y * speed;
        }
        if ( fmod(( pic_pos.x + (pac_radius - GRID_SIZE / 2) ), GRID_SIZE) == 0 &&
            fmod(( pic_pos.y + (pac_radius - GRID_SIZE / 2) - 90 ), GRID_SIZE) == 0) {
            grid_pos += direction;
            // check whether Pacman eating any dot or energizer
            if ( eat_dots() )   res = "eat dot";
            else if ( eat_energizer() ) res = "eat energizer";
        }

    }
    return res;
}
// prevent Pacman from moving through any walls or ghsot house entraces
bool PacMan::movable()
{
    for ( auto& wall : walls ){
        if ( grid_pos + direction == wall ) {
            direction = Vector2<int> ( 0, 0 );
            return false;
        }
    }
    for ( auto& e : ghost_en )
        if ( grid_pos + direction == e ) {
            direction = Vector2<int> ( 0, 0 );
            return false;
        }
    return true;
}
// check Pacman and dots position
bool PacMan::eat_dots()
{
    for ( int i = 0 ; i < dots->size(); i++ ) {
        if ( grid_pos == (*dots)[i] ){
            dots->erase( dots->begin() + i );
            return true;
        }
    }
    return false;
}
// check Pacman and energizers position
bool PacMan::eat_energizer()
{
    for ( int i = 0 ; i < energ->size(); i++ )
        if ( grid_pos == (*energ)[i] ){
               energ->erase( energ->begin() + i );
            return true;
        }
    return false;
}
// perform when Pacman hit any ghosts
void PacMan::die()
{
    if ( life > 0 )
        life -= 1;
    status = "dying";
    death_frame = 0;
    speed = 0;
}
// perform when the game restarted
void PacMan::restart( vector<Vector2<int>>& w, vector<Vector2<int>>& d,
vector<Vector2<int>>& e, vector<Vector2<int>>& gh )
{
    grid_pos = start_pos;
    pic_pos.x = grid_pos.x * GRID_SIZE - (pac_radius - GRID_SIZE / 2);
    pic_pos.y = grid_pos.y * GRID_SIZE - (pac_radius - GRID_SIZE / 2) + 90;
    move_count = 0;
    direction = LEFT;
    status = "alive";
    state = "left";
    speed = CHARACTER_SPEED;
    walls = w;
    dots = &d;
    energ = &e;
    ghost_en = gh;
    life = 3;
    texture.loadFromFile( "PacSprite/pacman0.png" );
    sprite.setTexture( texture );
    sprite.setPosition( pic_pos );
    death_frame = 0;
}
// revive Pacman if there is any life left
void PacMan::revive()
{
    grid_pos = start_pos;
    pic_pos.x = grid_pos.x * GRID_SIZE - (pac_radius - GRID_SIZE / 2);
    pic_pos.y = grid_pos.y * GRID_SIZE - (pac_radius - GRID_SIZE / 2) + 90;
    state = "left";
    direction = LEFT;
    move_count = 0;
    speed = CHARACTER_SPEED;
}
// load sprite path
void PacMan::load_elements()
{
    sprite_path_list.right.push_back("PacSprite/pacman0.png");
    sprite_path_list.right.push_back(string("PacSprite/pacright1.png"));
    sprite_path_list.right.push_back(string("PacSprite/pacright2.png"));
    
        sprite_path_list.left.push_back(string("PacSprite/pacman0.png"));
    sprite_path_list.left.push_back(string("PacSprite/pacleft1.png"));
    sprite_path_list.left.push_back(string("PacSprite/pacleft2.png"));
    
    sprite_path_list.up.push_back(string("PacSprite/pacman0.png"));
    sprite_path_list.up.push_back(string("PacSprite/pacUp1.png"));
    sprite_path_list.up.push_back(string("PacSprite/pacUp2.png"));
    
    sprite_path_list.down.push_back(string("PacSprite/pacman0.png"));
    sprite_path_list.down.push_back(string("PacSprite/pacdown1.png"));
    sprite_path_list.down.push_back(string("PacSprite/pacdown2.png"));
    
    death_sprite_path.push_back(string("PacSprite/death1.png"));
    death_sprite_path.push_back(string("PacSprite/death2.png"));
    death_sprite_path.push_back(string("PacSprite/death3.png"));
    death_sprite_path.push_back(string("PacSprite/death4.png"));
    death_sprite_path.push_back(string("PacSprite/death5.png"));
    death_sprite_path.push_back(string("PacSprite/death6.png"));
    death_sprite_path.push_back(string("PacSprite/death7.png"));
    death_sprite_path.push_back(string("PacSprite/death8.png"));
    death_sprite_path.push_back(string("PacSprite/death9.png"));
    death_sprite_path.push_back(string("PacSprite/death10.png"));
    death_sprite_path.push_back(string("PacSprite/death11.png"));
    death_sprite_path.push_back(string("PacSprite/death11.png"));
    death_sprite_path.push_back(string("PacSprite/death11.png"));
}
// draw Pacman on the window
void PacMan::draw( RenderWindow& window )
{
    if ( status != "dying" ) {
        if ( move_count + 1 >= 12 )
            move_count = 0;
        
        sprite.setPosition( pic_pos );
        if ( state == "left" )
            texture.loadFromFile( sprite_path_list.left[move_count / 4].c_str());
        else if ( state == "right" )
            texture.loadFromFile( sprite_path_list.right[move_count / 4].c_str());
        else if ( state == "up" )
            texture.loadFromFile( sprite_path_list.up[move_count / 4].c_str());
        else if ( state == "down" )
            texture.loadFromFile( sprite_path_list.down[move_count / 4].c_str());
        sprite.setTexture( texture );
        window.draw( sprite );
        move_count++;
    }
}
// draw Pacman on the window when it died
void PacMan::draw_die( RenderWindow& window )
{
    if ( death_frame <= 51 ) {
        texture.loadFromFile( death_sprite_path[death_frame / 4 ]);
        sprite.setTexture( texture );
        window.draw( sprite );
    }
    death_frame++;
}
