#include <SFML/Graphics.hpp>
#include "Headers/GameBox.h"
#include "Headers/Block.h"
#include "L_Block.cpp"

int main()
{
    unsigned char row = 20;
    unsigned char column = 10;
    unsigned char cell = 8;
    unsigned char screen_size = 4;
    unsigned char frame = 16667;

    GameBox gameBox;
    gameBox.innit();
    gameBox.display();

    L_block L;

    sf::RenderWindow window(sf::VideoMode(cell * column * screen_size, cell * row * screen_size), "Tetris", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0,  cell * column, cell * row)));

    sf::RectangleShape cellShape(sf::Vector2f(cell - 1, cell - 1));
    cellShape.setFillColor(sf::Color(36, 36, 88));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color(0,0,0));
        gameBox.draw_cell(window);
        L.draw(window);
        // for (int i = 0; i < column; i++)
        // {
        //     for (int j = 0; j < row; j++)
        //     {
        //         sf::Vector2f position(cell * i, cell * j);
        //         cellShape.setPosition(position);
        //         window.draw(cellShape);
        //     }
        // }
        window.display();
    }
    window.close();
    return 0;
}
