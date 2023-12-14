#include "GameBox.h"


void GameBox::innit()
{
    box = std::vector<std::vector<unsigned int>>(row, std::vector<unsigned int>(column, 9));
}

void GameBox::display()
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            cout << box[i][j] << " ";
        }
        cout << endl;
    }
}
vector <Color> GameBox::get_cell_colors()
{
    Color light_blue(0, 219, 255);
    Color blue(0, 36, 255);
    Color orange(255, 146, 0);
    Color yellow(255, 219, 0);
    Color light_green(0, 219, 0);
    Color purple(146, 0, 255);
    Color red(219, 0, 0);
    Color grey(73, 73, 85);
    Color dark_blue(36, 36, 85);
    Color background_cell(35, 26, 88);
    return { light_blue,blue,orange,yellow,light_green,purple,red,grey, dark_blue,background_cell };
}
void GameBox::draw_cell(RenderWindow& window)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            int cell_value = box[i][j];
            // trừ một ở đây nhằm làm tăng độ chính xác của pixel khi hiển thị trong box chơi game
            RectangleShape cell(Vector2f(cell_size - 1, cell_size - 1));

            cell.setPosition(static_cast<float>(cell_size * j) + 1, static_cast<float>(cell_size * i) + 1);
            cell.setFillColor(colors[cell_value]);
            window.draw(cell);
        }
    }
}
// check xem có cell nào vượt ra ngoài box không

bool GameBox::is_cell_outside_box(int row, int column)
{
    //check row
    if (row < 0 || row >= this->row)
        return true;
    if (column < 0 || column >= this->column)
        return true;
    return false;
}

bool GameBox::is_empty_cell(int row, int col)
{
    if (box[row][col] == 9)
    {
        return true;
    }
    return false;
}
int GameBox::number_completed_row()
{
    int res = 0;
    for (int i = 0; i < row; i++)
    {
        int count = 0;
        for (int j = 0; j < column; j++)
        {
            if (box[i][j] != 9)
            {
                count++;
            }
        }
        if (count == column)
        {
            res++;
        }
    }
    return res;
}
void GameBox::move_row(int row_index, int completed)
{
    for (int i = 0; i < column; i++)
    {
        box[row_index + completed][i] = box[row_index][i];
        // box[row_index][i] = 9;
    }
}
void GameBox::clear_row(int row)
{
    for (int i = 0; i < column; i++)
    {
        box[row][i] = 9;
    }
}
bool GameBox::is_completed_row(int row)
{
    int count = 0;
    for (int i = 0; i < column; i++)
    {
        if (box[row][i] == 9)
            return false;
    }

    return true;

}
void GameBox::update_box()
{
    int count = 0;
    for (int i = row - 1; i >= 0; i--)
    {
        if (is_completed_row(i))
        {
            count++;
            clear_row(i);
        }
        else {
            move_row(i, count);
        }
    }
}

void GameBox::game_over()
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (box[i][j] != 9)
            {
                box[i][j] = 7;
            }
        }
    }
}