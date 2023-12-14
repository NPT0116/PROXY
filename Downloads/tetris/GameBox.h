#pragma once
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
class GameBox
{
private:
    unsigned int row = 20;
    unsigned int column = 10;
    unsigned int cell_size = 8;
    vector <Color> colors;
public:
    bool is_cell_outside_box(int row, int column);
    GameBox() {
        colors = get_cell_colors();
    };
    bool is_empty_cell(int row, int col);
    void display();
    vector<vector<unsigned int> > box;
    void innit();
    void set_colors();
    static vector<Color> get_cell_colors();
    void draw_cell(RenderWindow& window);
    void lock_block();
    int number_completed_row();
    void move_row(int row_index, int completed);
    void clear_row(int row);
    void update_box();
    bool is_completed_row(int row);
    void game_over();
};