#include "Headers/GameBox.h"

void GameBox :: innit()
{
box = std::vector<std::vector<unsigned int>>(row, std::vector<unsigned int>(column, 0));
}

void GameBox :: display()
{
    for (int i = 0 ; i < row ; i++)
    {
        for (int j = 0 ; j < column; j++)
        {
            cout<<box[i][j]<<" ";
        }   
        cout<<endl;
    }
}
vector <Color> GameBox:: get_cell_colors()
{
    Color dark_blue(36, 36, 85);
    Color light_blue(0, 219, 255);
    Color blue(0, 36, 255);
    Color orange(255, 146, 0);
    Color yellow(255, 219, 0);
    Color light_green(0, 219, 0);
    Color purple(146, 0, 255);
    Color red(219, 0, 0);
    Color grey(73, 73, 85);
    return {dark_blue,light_blue,blue,orange,yellow,light_green,purple,red,grey};
}
void GameBox ::  draw_cell(RenderWindow &window)
{
    for (int i = 0 ; i < row ; i++)
    {
        for (int j = 0 ; j < column; j++)
        {
            int cell_value = box[i][j];
            // trừ một ở đây nhằm làm tăng độ chính xác của pixel khi hiển thị trong box chơi game
            RectangleShape cell(Vector2f(cell_size - 1, cell_size - 1));

        	cell.setPosition(static_cast<float>(cell_size * j ), static_cast<float>(cell_size * i ));
            cell.setFillColor(colors[cell_value]);
            window.draw(cell);
        }
    }
}

