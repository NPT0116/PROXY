#include"Block.h"


Block::Block()
{
    rotate = 0;
    id = -1;
    cell_size = 8;
}


void Block::draw(RenderWindow& window)
{
    vector<Position> pos = get_curr_position();
    RectangleShape cell(Vector2f(cell_size - 1, cell_size - 1));

    for (Position po : pos)
    {
        cell.setPosition(static_cast<float>(cell_size * po.col) + 1, static_cast<float>(cell_size * po.row) + 1);
        cell.setFillColor(colors[id]);

        window.draw(cell);
    }
}

void Block::move(int row, int col)
{
    row_margin += row;
    col_margin += col;
    // cout<<row_margin<<" "<<col_margin<<endl;
}

vector <Position> Block::get_curr_position()
{
    vector <Position> pos = block[rotate];
    vector <Position> curr_pos;
    for (Position po : pos)
    {
        Position x = Position(po.row + row_margin, po.col + col_margin);
        curr_pos.push_back(x);
    }
    return curr_pos;
}
void Block::rotate_block()
{
    rotate++;
    if (rotate == 4)
    {
        rotate = 0;
    }
}

