#include"tetris.hpp"
#include<cstdlib>
#include<ctime>
#include<iostream>

int main()
{
    std::srand(std::time(0));
    Tetris tetris;
    tetris.run();
    return 0;
}
