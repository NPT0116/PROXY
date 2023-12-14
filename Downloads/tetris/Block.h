#pragma once
#include <map>
#include "GameBox.h"
#include "Position.h"
#include "SFML/Graphics.hpp"
class Block
{
protected:
    map <int, vector <Position>> block;
    int cell_size;
    vector <Color > colors = GameBox::get_cell_colors();
    int rotate;
public:
    //độ dời của hàng và cột dùng cho hàm move
    int id;
    void rotate_block();
    int row_margin = 0;
    int col_margin = 0;
    Block();
    void draw(RenderWindow& window);
    void move(int, int);
    //hàm dùng để lấy vị trí hiện tại của block
    vector <Position> get_curr_position();
};