#pragma once
#include "Block_Type.cpp"
#include "Position.h"
#include <vector>
#include <time.h>
using namespace std;
class Tetris
{
private:
    Block currBlock;
    Block nextBlock;
public:
    GameBox box;
    bool gameOver = false;
    vector <Block>blocks;
    vector <Block>get_all_type_block();
    Tetris();
    //trả về địa chỉ của 1 block.
    Block randomBlock();
    void hand_input(Event& event);
    void moveBlockLeft();
    void moveBlockRight();
    void moveBlockDown();
    void Draw(RenderWindow& window);
    void moveBlockSpace();
    bool is_block_outside();
    void rotate_block();
    void lock_block();
    bool is_block_fit();
    void check_completed_row();
    void game_over();
    void restart_game();
};

