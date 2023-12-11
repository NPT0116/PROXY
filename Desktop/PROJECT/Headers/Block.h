#pragma once
#include <map>
#include "GameBox.h"
#include "Position.h"
#include "SFML/Graphics.hpp"
class Block
{
protected:
    int rotate;
    map <int,vector <Position>> block;
    int id  ;
    vector <Color > colors = GameBox::get_cell_colors();
    int cell_size;
public:
    Block();
    void draw(RenderWindow &window);
};