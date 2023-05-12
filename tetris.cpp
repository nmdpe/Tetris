#include"tetris.hpp"
#include<iostream>

Tetris::Tetris()
{
    window.create(sf::VideoMode(360, 720),"Tetris",sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(sf::Vector2i(100,100));

    back.loadFromFile("../resources/img/background.png");
    back_sp.setTexture(back);
    tile.loadFromFile("../resources/img/squares.png");
    tile_sp.setTexture(tile);
    tile_sp.setTextureRect(sf::IntRect(0,0,36,36));

    font.loadFromFile("../resources/font/font.ttf");

    score_txt.setFont(font);
    score_txt.setPosition(150.f,10.f);
    score_txt.setString("SCORE: " + std::to_string(score));
    score_txt.setCharacterSize(15);
    score_txt.setOutlineThickness(3);

    game_over_txt.setFont(font);
    game_over_txt.setPosition(60.f,300.f);
    game_over_txt.setString("PRESS 123\nTO CHOOSE\nDIFFICULTY");
    game_over_txt.setCharacterSize(40);
    game_over_txt.setOutlineThickness(3);

    record_txt.setFont(font);
    record_txt.setPosition(30.f,500.f);
    record_txt.setString("NEW RECORD!");
    record_txt.setCharacterSize(40);
    record_txt.setOutlineThickness(3);

    high_score_txt.setFont(font);
    high_score_txt.setPosition(110.f,40.f);
    high_score_txt.setCharacterSize(15);
    high_score_txt.setOutlineThickness(3);

    record_sound.openFromFile("../resources/music/EZ4ENCE.wav");

    block.resize(4);
    a.resize(10);
    for(auto i=0;i<10;++i) 
    {
        a[i].resize(20);
        for(auto j=0;j<20;++j) a[i][j]=0;
    }
    max_score.resize(3);
    for(auto &c:max_score) c=0;
}

void Tetris::generate_block()
{
    out_range_flag=0;
    shape=rand()%7;
    int x{},y{};
    for(auto i=0;i<4;++i)
    {
        x=block[i].first=xy_calc[shape][i]%2;
        y=block[i].second=xy_calc[shape][i]/2;
        if(a[x][y]) game_over_flag=1;
    }
}

int Tetris::out_range()
{
    for(auto i=0;i<4;++i)
    {
        int x=block[i].first,y=block[i].second;
        if(x<0||x>=10||y>=20||a[x][y]) return 1;
    } 
    return 0;
}

void Tetris::move_lr(int x)
{
    dead_block=block;
    for(auto i=0;i<4;++i) block[i].first+=x;
    if(out_range())
    {
        //out_range_flag=1;
        block=dead_block;
    }
}

void Tetris::move_down()
{
    //if(out_range_flag) return;
    dead_block=block;
    if(time_count>tick_time)
    {
        for(auto i=0;i<4;++i) ++block[i].second;
        time_count=0;
    }
        
    if(out_range())
    {
        out_range_flag=1;
        block=dead_block;
    }
}

void Tetris::rotate()//逆
{
    //if(out_range_flag) return;
    dead_block=block;
    int px=block[1].first,py=block[1].second;//旋转中心
    for(auto i=0;i<4;++i)
    {
        int x=block[i].first,y=block[i].second;
        block[i].first=px-y+py;
        block[i].second=py+x-px;
    }
    if(out_range())
    {
        block=dead_block;
    }
}

void Tetris::game_over()
{
    if(out_range_flag)
    {
        for(int i=0;i<4;++i) a[block[i].first][block[i].second]=shape+1;
        generate_block();
    }
}

void Tetris::clean_line()
{
    int k=19;
    for(int j=19;j>=0;--j)
    {
        int count=0;
        for(int i=0;i<10;++i)
        {
            if(a[i][j]) ++count;
            a[i][k]=a[i][j];
        }
        if(count<10) --k;
        else 
        {
            ++score;
            if(score>max_score[level-1])
            {
                max_score[level-1]=score;
                highest_flag=1;
            } 
        }
    }
}

void Tetris::draw()
{
    window.clear(sf::Color::Black);
    window.draw(back_sp);
    
    for(auto i=0;i<10;++i)
        for(auto j=0;j<20;++j)
            if(a[i][j])
            {
                tile_sp.setTextureRect(sf::IntRect(a[i][j]*36,0,36,36));
                tile_sp.setPosition(i*36,j*36);
                window.draw(tile_sp);
            }
    for(auto i=0;i<4;++i)
    {
        tile_sp.setTextureRect(sf::IntRect((shape+1)*36,0,36,36));
        tile_sp.setPosition(block[i].first*36,block[i].second*36);
        window.draw(tile_sp);
    }
    score_txt.setString("SCORE: " + std::to_string(score));
    window.draw(score_txt);
    high_score_txt.setString("LEVEL"+std::to_string(1)+" RECORD: " + std::to_string(max_score[0])
                            +"\nLEVEL"+std::to_string(2)+" RECORD: " + std::to_string(max_score[1])
                            +"\nLEVEL"+std::to_string(3)+" RECORD: " + std::to_string(max_score[2]));
    window.draw(high_score_txt);
    
    
    if(game_over_flag) 
    {
        window.draw(game_over_txt);
        if(highest_flag) 
        {
            window.draw(record_txt);
            //record_sound.play();
            //std::cout<<record_sound.getStatus()<<'\n';
        }
    }
    window.display();   
}

void Tetris::new_game()
{
    for(auto i=0;i<10;++i)
        for(auto j=0;j<20;++j) a[i][j]=0;
    generate_block();
    game_over_flag=0;
    highest_flag=0;
    score=0;
}

void Tetris::run()
{
    game_over_flag=1;
    while(window.isOpen())
    {
        tick_time=static_cast<double>(4-level)/10.0;
        double time=clock.getElapsedTime().asSeconds();
        clock.restart();
        time_count+=time;
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type==sf::Event::Closed) window.close();
            if(game_over_flag)
            {
                if(event.type==sf::Event::KeyPressed)
                {
                    if(event.key.code==sf::Keyboard::Num1) level=1;
                    else if(event.key.code==sf::Keyboard::Num2) level=2;
                    else if(event.key.code==sf::Keyboard::Num3) level=3;
                    else continue;
                    new_game(); 
                }
            }
        /*    if(game_over_flag)
                if(event.type==sf::Event::KeyPressed) new_game();
                */
            if(event.type==sf::Event::KeyPressed)
            {
                if(event.key.code==sf::Keyboard::Right) move_lr(1);
                else if(event.key.code==sf::Keyboard::Left) move_lr(-1);
                else if(event.key.code==sf::Keyboard::Up) rotate();
                else if(event.key.code==sf::Keyboard::Down) tick_time/=5;                   
            }
        }
        if(!game_over_flag)
        {
            move_down();
            clean_line();
            game_over();
        }
        draw();
    }
}
