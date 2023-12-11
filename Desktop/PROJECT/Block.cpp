#include"Headers/Block.h"


Block :: Block()
{
    int rotate = 0 ;
    id  = -1;
    cell_size = 8;
}


void Block :: draw (RenderWindow &window)
{
    vector<Position> pos = block[rotate];
    RectangleShape cell(Vector2f(cell_size - 1, cell_size - 1));
    for (Position po : pos)
    {
        cell.setPosition(static_cast<float>(cell_size * po.col  ), static_cast<float>(cell_size * po.row ));
        cell.setFillColor(colors[id + 2]);
        window.draw(cell);
    }
}