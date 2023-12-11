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
    GameBox(){
        colors = get_cell_colors();
    };
    void display();
    vector<vector<unsigned int> > box;
    void innit();
    void set_colors();
	static vector<Color> get_cell_colors();
    void draw_cell(RenderWindow &window);
};