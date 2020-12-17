
#include "GameClass.hpp"

// constructor of class Game
Game::Game()
{
    win = 0;
    reviving = true;
    point = 0;
    score_text = Text ( string("SCORE: ") + to_string( point ),  arcade_font, 44 );
    life_text = Text ( string("LIFE: "),  arcade_font, 44 );

    maze.setPosition( 0, 90 );
    scene = "menu";
    dot = CircleShape(6);
    energizer_dot = CircleShape(14);

    start_die = Time( Time::Zero );
}

void Game::run()
{
    // load maze information and other resource files
    try {
    load_elements();
    }
    catch ( invalid_argument& e ) {
        cout << e.what() << endl;
        return;
    }

    sound.setBuffer( beginning_sound );
    maze.setTexture( maze_texture );
    background.setTexture( menu_texture );
    
    // used for checking whether Pacman is eating dot or hit any ghosts
    string pac_update_status;
    // next Pacman turning direction
    Vector2<int> pac_next_turn = LEFT;
    play = 1;
    // create start button and exit button in menu scene
    Button start_button ( SCREEN_WIDTH / 2 - 200, 600, 400, 160, "start", &pac_font,
                         Color( 58, 174, 89 ), Color( 88, 204, 119 ), Color( 252,184,40 ), 74 );
    Button exit_button ( 50, 1200, 300, 80, "exit", &pac_font, Color::Red, Color::Red, Color::White, 40 );

    // create ghosts and Pacman objects
    Blinky blinky( start_pos.blinky, walls, ghost_dec, ghost_en );
    Pinky pinky( start_pos.pinky, walls, ghost_dec, ghost_en );
    Clyde clyde( start_pos.clyde, walls, ghost_dec, ghost_en );
    Inky inky( start_pos.inky, walls, ghost_dec, ghost_en );
    PacMan pac ( start_pos.pac, walls, dots, energ, ghost_en );
    // vector of all ghosts : blinky - pinky - inky - clyde
    ghosts.push_back( &blinky );
    ghosts.push_back( &pinky );
    ghosts.push_back( &inky );
    ghosts.push_back( &clyde );
    
    // load sprite paths to every objects
    for ( auto& g : ghosts )
        g->load_elements();
    pac.load_elements();
    // create program window
    RenderWindow window( VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "Pac Man", Style::Close );
    window.setFramerateLimit( FPS );

    // main loop
    while ( window.isOpen() )
    {
        if ( scene == "menu" ) {
            background.setTexture( menu_texture );
            window.clear();
            window.draw( background );
            exit_button.render( &window );
            start_button.render( &window );
            window.display();
            start_button.update( Vector2f(Mouse::getPosition( window ).x, Mouse::getPosition( window ).y) );
            exit_button.update( Vector2f(Mouse::getPosition( window ).x, Mouse::getPosition( window ).y) );
            // start the game
            if ( start_button.isPressed() ) {
                game_time = program_clock.getElapsedTime();
                scene = "playing";
                window.clear();
                window.draw( maze );
                sound.setBuffer( beginning_sound );
                sound.play();
                draw_dots( window );
                window.display();
                for ( auto& g : ghosts )
                    g->start_mode = milliseconds(program_clock.getElapsedTime().asMilliseconds() + 4150);
            }
            // exit program and close window
            else if ( exit_button.isPressed() ) {
                break;
            }

            Event event;
            while( window.pollEvent( event )){
                if ( event.type == Event::Closed )
                    window.close();
            }
            
        }
        else if ( scene == "playing" ) {
            // play the beginning sound at the beginning
            if ( play == 0 && win == 0 ) {
                sound.setBuffer( beginning_sound );
                sound.play();
                play = 1;
            }
            else {
                // game started
            if ( sound.getStatus() == SoundSource::Playing
                && sound.getBuffer() == &beginning_sound )
                // draw an empty maze with bgm
                prepare_game( window, pac.life );
            else {
                // playing scene
                if (  sound.getStatus() == SoundSource::Playing
                    && sound.getBuffer() == &death_sound) {
                    // when Pacman is dead play dying sound and draw death scene
                    redraw_game( pac, window, true );
                    reviving = false;
                }
                
                else if ( !reviving ) {
                    reviving = true;
                    start_die = Time( Time::Zero );
                    game_time = program_clock.getElapsedTime();
                    play = 0;
                    pac.revive();
                    pac_next_turn = LEFT;
                    for ( auto& ghost : ghosts )
                        ghost->pac_revive();
                }
                else {
                    pac.set_status( "alive" );
                    Event event;
                    while( window.pollEvent( event )){
                        if ( event.type == Event::Closed )
                            window.close();
                        // user interactive
                        else if ( event.type == Event::KeyPressed ) {
                            switch ( event.key.code ) {
                                case sf::Keyboard::Left:
                                    pac_next_turn = LEFT;
                                    break;
                                case sf::Keyboard::Right:
                                    pac_next_turn = RIGHT;
                                    break;
                                case sf::Keyboard::Up:
                                    pac_next_turn = UP;
                                    break;
                                case sf::Keyboard::Down:
                                    pac_next_turn = DOWN;
                                    break;
                            }
                        }
                    }
                    // if Pacman can travel along next direction : change pac direction
                    if ( pac.turnable( pac_next_turn ) ){
                        pac.pac_move( pac_next_turn );
                    }
                    // win = 0 is playing state
                    if ( win == 0 ) {
                        // update ghosts and Pacman position and status
                        for ( auto& i : ghosts )
                            i->update( pac.get_grid_pos(), pac.get_direction(), blinky.get_grid_pos() );
                        
                        vector<Vector2<int>> temp{ blinky.get_grid_pos(), pinky.get_grid_pos(),
                            inky.get_grid_pos(), clyde.get_grid_pos() };
                        
                        pac_update_status = pac.update( temp );
                        hit_ghost( pac );
                        if ( pac.life > 0 ) {
                            if ( pac_update_status == "eat dot" ) {
                                point += 10;
                                if ( sound.getStatus() != SoundSource::Playing ){
                                    sound.setBuffer( eat_dot_sound );
                                    sound.play();
                                }
                            }
                            else if ( pac_update_status == "eat energizer" ) {
                                sound.setBuffer( eat_en_sound );
                                sound.play();
                                for ( auto& ghost : ghosts )
                                    ghost->change_mode( "frighten" );
                            }
                        }
                        // win = -1 is lost
                        else    win = -1;
                        // win = 1 is won
                        if ( dots.size() == 0 )
                            win = 1;
                        redraw_game( pac, window, false );
                    }
                    // if player either win or lose : change to ending scene
                    else if ( win == 1 || win == -1 )
                        scene = "end";
                }
            }
            }
        }
        // draw the ending scene and check whether player wants to play again or exit the program
        else if ( scene == "end" ) {
            Event event;
            while( window.pollEvent( event )){
                if ( event.type == Event::Closed )
                    window.close();
                else if ( event.type == Event::KeyPressed ) {
                    if ( event.key.code == sf::Keyboard::Space ) {
                        sound.setBuffer( beginning_sound );
                        sound.play();
                        restart_game( pac );
                        scene = "playing";
                    }
                    else if ( event.key.code == sf::Keyboard::Escape ) {
                        restart_game( pac );
                        scene = "menu";
                    }
                }
            }
            draw_ending( window );
        }

    }
}

// draw every dots and energizers on the maze
void Game::draw_dots( RenderWindow& window )
{
    for ( const auto& i : dots ) {
        dot.setPosition( i.x * GRID_SIZE + GRID_SIZE / 2 - 6, i.y * GRID_SIZE + GRID_SIZE / 2 + 79);
        window.draw(dot);
    }
    for ( const auto& i : energ ) {
        energizer_dot.setPosition( i.x * GRID_SIZE + GRID_SIZE / 2 - 14, i.y * GRID_SIZE + GRID_SIZE / 2 + 71 );
        window.draw( energizer_dot );
    }
}
// load sounds, maze informations, and images
void Game::load_elements()
{
    maze_texture.loadFromFile( "resources/maze.png" );
    ifstream maze_grid( "resources/maze.txt" );
    if ( !maze_grid ) {
        throw invalid_argument( "Failed to open maze text file!" );
    }
    char temp;
    // read an information from maze text file
    for ( int y = 0; y < 31; y++ ) {
        for ( int x = 0; x < 28; x++ ) {
            maze_grid >> temp;
            if ( temp == '1' )
                walls.push_back( Vector2<int>( x, y ) );
            else if ( temp == 'D' )
                dots.push_back( Vector2<int>( x, y ) );
            else if ( temp == 'E' )
                energ.push_back( Vector2<int>( x, y ) );
            else if ( temp == 'B' )
                start_pos.blinky = Vector2<int>( x, y );
            else if ( temp == 'I' )
                start_pos.inky = Vector2<int>( x, y );
            else if ( temp == 'P' )
                start_pos.pinky = Vector2<int>( x, y );
            else if ( temp == 'C' )
                start_pos.clyde = Vector2<int>( x, y );
            else if ( temp == 'M' )
                start_pos.pac = Vector2<int>( x, y );
            else if ( temp == 'G' )
                ghost_en.push_back( Vector2<int>( x, y ) );
            else if ( temp == 'K' )
                ghost_dec.push_back( Vector2<int>( x, y ) );
            else if ( temp == 'X' ) {
                ghost_dec.push_back( Vector2<int>( x, y ) );
                dots.push_back( Vector2<int>( x, y ) );
            }
        }
        maze_grid.get();
    }
    maze_grid.close();

    pac_font.loadFromFile( "resources/PAC-FONT.TTF" );
    arcade_font.loadFromFile( "resources/ARCADE_R.TTF" );
    
    menu_texture.loadFromFile( "resources/menuscene.png" );
    win_texture.loadFromFile( "resources/winscene.png" );
    lose_texture.loadFromFile( "resources/losescene.png" );

    eat_dot_sound.loadFromFile( "Sounds/pacman_chomp.wav" );
    eat_en_sound.loadFromFile( "Sounds/pacman_eatfruit.wav" );
    death_sound.loadFromFile( "Sounds/pacman_death.wav" );
    beginning_sound.loadFromFile( "Sounds/pacman_beginning.wav" );
    eat_ghost_sound.loadFromFile( "Sounds/pacman_eatghost.wav" );
}
// check whether Pacman hit any ghosts or not
void Game::hit_ghost ( PacMan& pac )
{
    for ( auto& ghost : ghosts ) {
        if ( ( abs(pac.get_pic_pos().x - ghost->get_pic_pos().x) <= 30 && abs(pac.get_pic_pos().y - ghost->get_pic_pos().y) <= 10 ) ||
            ( abs(pac.get_pic_pos().y - ghost->get_pic_pos().y) <= 30 &&  abs(pac.get_pic_pos().x - ghost->get_pic_pos().x) <= 10) ) {
            // if ghost is frightening : eat it
            if ( ghost->get_mode() == "frighten" ) {
                sound.setBuffer( eat_ghost_sound );
                sound.play();
                ghost->set_pic_pos( ghost->find_pic_pos() );
                ghost->change_mode( "eaten" );
                point += 750;
            }
            // if ghost is not eaten and not frightening : Pacman will die
            else if ( ghost->get_mode() != "eaten" ) {
                sound.setBuffer( death_sound );
                sound.play();
                pac.die();
                start_die = program_clock.getElapsedTime();
            }
        }
    }
}
// restart the game
void Game::restart_game( PacMan& pac )
{
    walls.clear();
    dots.clear();
    energ.clear();
    ghost_en.clear();
    ghost_dec.clear();
    point = 0;
    win = 0;
    game_time = program_clock.getElapsedTime();
    load_elements();
    pac.restart( walls, dots, energ, ghost_en );
    start_die = Time( Time::Zero );
    for ( auto& g : ghosts )
        g->restart( walls, ghost_dec, ghost_en);
}
// draw preparing scene
void Game::prepare_game( RenderWindow& window, int paclife )
{
    window.clear();
    window.draw( maze );
    draw_dots( window );
    draw_score_life( window, paclife );
    window.display();
}
// redraw every scene in the game
void Game::redraw_game( PacMan& pac, RenderWindow& window, bool die )
{
    window.clear();
    window.draw( maze );
    draw_dots( window );
    for ( auto& g : ghosts )
        g->draw( window );
    if ( die )  pac.draw_die( window );
    else pac.draw( window );
    draw_score_life( window, pac.life );
    window.display();
}
// draw ending scene
void Game::draw_ending( RenderWindow& window )
{
    Text text1( "SCORE:", arcade_font, 60);
    Text text2( to_string( point ), arcade_font, 100 );
    text1.setFillColor( Color::White );
    text1.setPosition(  600, 100 );
    text2.setPosition( 600, 210 );
    window.clear();
    if ( win == 1 )
    {
        background.setTexture( win_texture );
        window.draw( background );
        text2.setFillColor( Color::Green );
    }
    else if ( win == -1 )
    {
        background.setTexture( lose_texture );
        window.draw( background );
        text2.setFillColor( Color::Red );
    }
    window.draw( text1 );
    window.draw( text2 );
    window.display();
}
// draw a score and life text above the maze
void Game::draw_score_life( RenderWindow& window, int paclife )
{
    istringstream temp( point );
    life_text.setString( "LIFE: " + to_string( paclife ) );
    life_text.setFillColor( Color::Green );
    life_text.setPosition( 1070 - life_text.getGlobalBounds().width, 20 );
    score_text.setString( string("SCORE: ") + to_string( point ) );
    score_text.setFillColor( Color::White );
    score_text.setPosition( 50, 20 );
    window.draw( score_text );
    window.draw( life_text );
}
