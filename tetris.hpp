#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<vector>
using std::vector;
using std::pair;

class Tetris
{
    public:
    Tetris();
    void run();

    private:
    int score{},shape{},level{};
    vector<int> max_score;
    int game_over_flag{},out_range_flag{},highest_flag{},new_flag{};
    double time_count{},tick_time{0.3};
    vector<pair<int,int> > block;
    vector<pair<int,int> > dead_block;
    vector<vector<int>> xy_calc
    {
        {1,3,5,7}, // I
        {2,4,5,7}, // S
        {3,5,4,6}, // Z
        {3,5,4,7}, // T
        {2,3,5,7}, // L
        {3,5,7,6}, // J
        {2,3,4,5}, // O    
    };
    vector<vector<int> > a;   
    sf::RenderWindow window;
    sf::Texture back,tile;
    sf::Sprite back_sp,tile_sp;
    sf::Clock clock;
    sf::Font font;
    sf::Text game_over_txt,score_txt,high_score_txt,record_txt,new_game_txt;
    sf::SoundBuffer buffer;
    sf::Music record_sound;

    void generate_block();
    int out_range();
    void rotate();
    void move_lr(int x);
    void game_over();
    void move_down();
    void clean_line();
    void new_game();
    void draw();
};
