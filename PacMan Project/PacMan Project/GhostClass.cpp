#include "GhostClass.hpp"

// constructor
// parameters : start position, wall position list, decision points position list,
//              ghost house entrance position list
Ghost::Ghost ( Vector2<int> start, vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e ) :
Character( start )
{
    srand( time(NULL) );
    grid_pos = start;
    pic_pos = find_pic_pos();
    move_count = 0;
    direction = LEFT;
    mode = "scatter";
    speed = CHARACTER_SPEED;
    walls = w;
    decision_points = d;
    house_entrance = e;
    target = Vector2<int> {0,0};
    start_mode = program_clock.getElapsedTime();
    in_house = true;
}

Blinky::Blinky ( Vector2<int> start, vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e )
: Ghost( start, w, d, e )
{
    direction = RIGHT;
    in_house = false;
}
// update ghosts mode and position
void Ghost::update( Vector2<int> pac_pos, Vector2<int> pac_direction, Vector2<int> blinky_pos )
{
    int timer = ( program_clock.getElapsedTime() - start_mode ).asSeconds();
    
    if ( mode == "scatter" ){
        if ( timer >= 7 )
            change_mode( "chase" );
    }
    else if ( mode == "chase" ) {
        if ( timer >= 20 )
            change_mode( "scatter" );
    }
    else if ( mode == "frighten" ) {
        if ( timer >= 10 )
            change_mode( "scatter" );
    }
    else if ( mode == "eaten" ) {
        if ( in_house )
            change_mode( "scatter" );
    }
    
    if ( movable() ) {
        if ( mode == "eaten" )
            if ( grid_pos == target )
                direction = DOWN;
        
        pic_pos.x += direction.x * speed;
        pic_pos.y += direction.y * speed;
                
        if ( fmod( pic_pos.x + 10, GRID_SIZE ) == 0 &&
            fmod( pic_pos.y - 85, GRID_SIZE ) == 0 ) {
            grid_pos += direction;
            
            for ( auto& e : house_entrance )
                if ( grid_pos == e ) {
                    last_dec_point = grid_pos;
                    in_house = !in_house;
                    break;
                }
            
            find_target( pac_pos, pac_direction, blinky_pos );
            
            for ( auto& d : decision_points )
                if ( grid_pos == d ) {
                    last_dec_point = grid_pos;
                    make_decision();
                }
        }
    }
    
    if ( in_house ) {
        for ( auto& e : house_entrance )
            if ( grid_pos + UP == e )
                direction = UP;
    }
    
}
// if ghost hit a wall : find another direction
bool Ghost::movable()
{
    for ( const auto& wall : walls )
        if ( grid_pos + direction == wall ) {
            direction = find_turn();
            return false;
        }
    return true;
}
// find new direction ( not opposite direction of current direction )
Vector2<int> Ghost::find_turn()
{
    vector<Vector2<int>> turns{ UP, LEFT, DOWN, RIGHT };
    for ( int i = 0; i < 4; i++ ){
        if ( turns[i] == direction ){
            turns.erase( turns.begin() + i );
            if ( i > 1 )
                turns.erase ( turns.begin() + i - 2 );
            else
                turns.erase ( turns.begin() + i + 1 );
        }
    }
    
    for ( int i = 0; i < 2; i++ )
        for ( auto& w : walls )
            if ( grid_pos.x + turns[i].x == w.x && grid_pos.y + turns[i].y == w.y )
                turns.erase ( turns.begin() + i );
    
    return turns[0];
}
// when ghost is on the decision point : find fastest direction to the target
void Ghost::make_decision()
{
    vector<Vector2<int>> turns{ UP, LEFT, DOWN, RIGHT };
    vector<double> distance;
    for ( int i = 0; i < 4; i++ ){
        if ( turns[i] == direction ){
            if ( i > 1 )
                turns.erase ( turns.begin() + i - 2 );
            else
                turns.erase ( turns.begin() + i + 2 );
        }
    }
    
    for ( int i = 0; i < 3; i++ )
        for ( auto& w : walls )
            if ( grid_pos + turns[i] == w ){
                turns.erase ( turns.begin() + i );
                break;
            }
    
    if ( mode == "frighten" ) {
        direction = turns[ rand() % turns.size() ];
    }
    else {
        for ( auto& t : turns )
            distance.push_back( find_distance( grid_pos + t, target ));
        
        int index = 0;
        for ( int i = 1; i < distance.size(); i++ ) {
            if ( distance[i] < distance[index] )
                index = i;
        }
        direction = turns[index];
    }
         
}
// find distance between two objects
double Ghost::find_distance( Vector2<int> start, Vector2<int> end )
{
    return sqrt( pow((start.x - end.x), 2) + pow((start.y - end.y), 2) );
}
// change ghost mode
void Ghost::change_mode ( string newM )
{
    mode = newM;
    start_mode = program_clock.getElapsedTime();
    // when ghost is change into frighten mode, ghost was turing around
    if ( mode == "frighten" ) {
        in_house = false;
        direction.x *= -1;
        direction.y *= -1;
        last_dec_point = Vector2<int> ( -100, -100 );
        speed = CHARACTER_SPEED / 2;
    }
    else if ( mode != "eaten" )  speed = CHARACTER_SPEED;
    else {
        speed = CHARACTER_SPEED * 2;
        last_dec_point = Vector2<int> ( -100, -100 );
        for ( auto& i : decision_points )
            if ( grid_pos == i )
                make_decision();
    }
    pic_pos = find_pic_pos();
}
// perform when Pacman is reviving ( reset the ghost position )
void Ghost::pac_revive()
{
    grid_pos = start_pos;
    pic_pos = find_pic_pos();
    mode = "scatter";
    direction = UP;
    in_house = true;
    last_dec_point = Vector2<int>( -100, -100 );
    start_mode = program_clock.getElapsedTime();
    speed = CHARACTER_SPEED;
}
// Blinky will target Pacman
void Blinky::find_target( Vector2<int> pac_pos, Vector2<int> pac_direction, Vector2<int> blinky_pos )
{
    if ( mode == "eaten" )
        target = Vector2<int>( 13, 11 );
    else if ( mode == "scatter" )
        target = Vector2<int>( 25, 0 );
    else if ( mode == "chase" )
        target = pac_pos;
}

void Blinky::pac_revive()
{
    Ghost::pac_revive();
    in_house = false;
    direction = RIGHT;
}
// Pinky will target next 4 grid in front of Pacman
void Pinky::find_target( Vector2<int> pac_pos, Vector2<int> pac_direction, Vector2<int> blinky_pos )
{
    if ( mode == "eaten" )
        target = Vector2<int>( 13, 11 );
    else if ( mode == "scatter" )
        target = Vector2<int>( 1, 0 );
    else if ( mode == "chase" ) {
        if ( pac_direction == UP )
            target = pac_pos + Vector2<int>( -4, -4 );
        else
            target = Vector2<int>( pac_pos.x + pac_direction.x * 4,
                                  pac_pos.y + pac_direction.y * 4 );
    }
}
    
void Inky::find_target( Vector2<int> pac_pos, Vector2<int> pac_direction,
                       Vector2<int> blinky_pos )
{
    if ( mode == "eaten" )
        target = Vector2<int>( 13, 11 );
    else if ( mode == "scatter" )
        target = Vector2<int>( 27, 30 );
    else if ( mode == "chase" ) {
        Vector2<int> temp_target, difference;
        // create temporary target, 2 grids in front of Pacman
        if ( pac_direction == UP )
            temp_target = pac_pos + Vector2<int> ( 2, 2 );
        else {
            temp_target = Vector2<int>( pac_pos.x + pac_direction.x * 2,
                                       pac_pos.y + pac_direction.y * 2 );
        }
        difference = Vector2<int> ( abs( blinky_pos.x - temp_target.x ),
                                   abs( blinky_pos.y - temp_target.y ));
        // the real target is opposite vector of temporary target and Blinky's position
        target = temp_target - difference;
    }
}
// Clyde will target Pacman if Pacman was closing with clyde
void Clyde::find_target( Vector2<int> pac_pos, Vector2<int> pac_direction,
                        Vector2<int> blinky_pos )
{
    if ( mode == "eaten" )
        target = Vector2<int> ( 13, 11 );
    else if ( mode == "scatter" )
        target = Vector2<int>( 0, 30 );
    else if ( mode == "chase" ) {
        if ( find_distance( grid_pos, pac_pos ) <= 8 )
            target = pac_pos;
        else
            target = Vector2<int>( 0, 30 );
    }
}
// perform while game was restarting
void Ghost::restart ( vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e )
{
    grid_pos = start_pos;
    pic_pos = find_pic_pos();
    move_count = 0;
    direction = LEFT;
    mode = "scatter";
    speed = CHARACTER_SPEED;
    walls = w;
    decision_points = d;
    house_entrance = e;
    target = Vector2<int> {0,0};
    start_mode = program_clock.getElapsedTime();
    in_house = true;
}

void Blinky::restart ( vector<Vector2<int>> w, vector<Vector2<int>> d, vector<Vector2<int>> e )
{
    Ghost::restart( w, d, e );
    direction = RIGHT;
    in_house = false;
}

// load sprite paths
void Ghost::load_elements()
{
    frighten_sprite_path.push_back(string("Frighten/frighten.png"));
    frighten_sprite_path.push_back(string("Frighten/frighten2.png"));
}

void Blinky::load_elements()
{
    Ghost::load_elements();
    sprite_path_list.right.push_back(string("Blinky/blinkyright.png"));
    sprite_path_list.right.push_back(string("Blinky/blinkyright2.png"));
    sprite_path_list.left.push_back(string("Blinky/blinkyleft.png"));
    sprite_path_list.left.push_back(string("Blinky/blinkyleft2.png"));
    sprite_path_list.up.push_back(string("Blinky/blinkyup.png"));
    sprite_path_list.up.push_back(string("Blinky/blinkyup2.png"));
    sprite_path_list.down.push_back(string("Blinky/blinkydown.png"));
    sprite_path_list.down.push_back(string("Blinky/blinkydown2.png"));
}

void Pinky::load_elements()
{
    Ghost::load_elements();
    sprite_path_list.right.push_back(string("Pinky/pinkyright.png"));
    sprite_path_list.right.push_back(string("Pinky/pinkyright2.png"));
     sprite_path_list.left.push_back(string("Pinky/pinkyleft.png"));
    sprite_path_list.left.push_back(string("Pinky/pinkyleft2.png"));
    sprite_path_list.up.push_back(string("Pinky/pinkyup.png"));
    sprite_path_list.up.push_back(string("Pinky/pinkyup2.png"));
    sprite_path_list.down.push_back(string("Pinky/pinkydown.png"));
    sprite_path_list.down.push_back(string("Pinky/pinkydown2.png"));
}

void Inky::load_elements()
{
    Ghost::load_elements();
     sprite_path_list.right.push_back(string("Inky/inkyright.png"));
     sprite_path_list.right.push_back(string("Inky/inkyright2.png"));
    sprite_path_list.left.push_back(string("Inky/inkyleft.png"));
    sprite_path_list.left.push_back(string("Inky/inkyleft2.png"));
    sprite_path_list.up.push_back(string("Inky/inkyup.png"));
    sprite_path_list.up.push_back(string("Inky/inkyup2.png"));
    sprite_path_list.down.push_back(string("Inky/inkydown.png"));
    sprite_path_list.down.push_back(string("Inky/inkydown2.png"));
}

void Clyde::load_elements()
{
    Ghost::load_elements();
     sprite_path_list.right.push_back(string("Clyde/clyderight.png"));
     sprite_path_list.right.push_back(string("Clyde/clyderight2.png"));
    sprite_path_list.left.push_back(string("Clyde/clydeleft.png"));
    sprite_path_list.left.push_back(string("Clyde/clydeleft2.png"));
    sprite_path_list.up.push_back(string("Clyde/clydeup.png"));
    sprite_path_list.up.push_back(string("Clyde/clydeup2.png"));
    sprite_path_list.down.push_back(string("Clyde/clydedown.png"));
    sprite_path_list.down.push_back(string("Clyde/clydedown2.png"));
}

// draw each ghosts on the screen
void Ghost::draw( RenderWindow& window )
{
    if ( move_count + 1 >= 8 )
        move_count = 0;
    
    if ( mode == "frighten" )
        texture.loadFromFile( frighten_sprite_path[ move_count / 4 ].c_str() );
    else if ( mode == "eaten" )
        texture.loadFromFile( "resources/eaten.png" );
    else {
        if ( direction == LEFT )
            texture.loadFromFile( sprite_path_list.left[ move_count / 4].c_str() );
        else if ( direction == RIGHT )
            texture.loadFromFile( sprite_path_list.right[ move_count / 4].c_str() );
        else if ( direction == UP )
            texture.loadFromFile( sprite_path_list.up[ move_count / 4].c_str() );
        else if ( direction == DOWN )
            texture.loadFromFile( sprite_path_list.down[ move_count / 4].c_str() );
    }
    sprite.setTexture( texture );
    move_count++;
    sprite.setPosition( pic_pos );
    window.draw( sprite );
}
