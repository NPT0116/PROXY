#include "Tetris.h"


vector <Block> Tetris::get_all_type_block()
{
    return { L_block(), J_block(), I_block(), O_block(), S_block(),T_block(), Z_block() };
}



Tetris::Tetris()
{
    blocks = { L_block(), J_block(), I_block(), O_block(), S_block(),T_block(), Z_block() };
    box.innit();
    currBlock = randomBlock();
    nextBlock = randomBlock();
}

Block  Tetris::randomBlock()
{
    if (blocks.empty())
    {
        blocks = { L_block(), J_block(), I_block(), O_block(), S_block(),T_block(), Z_block() };
    }
    srand(time(NULL));
    rand();
    int index = rand() % blocks.size();

    Block randBlock = blocks[index];
    blocks.erase(blocks.begin() + index);
    return  randBlock;
}


void Tetris::hand_input(Event& event)
{
    // xem coi sự kiện có phải là keypress không

    switch (event.key.code)
    {

    case Keyboard::Left:
        moveBlockLeft();
        break;
    case Keyboard::Right:
        moveBlockRight();
        break;
    case Keyboard::Down:
        moveBlockDown();
        break;
    case Keyboard::Up:
        rotate_block();
        break;
    case Keyboard::Space:
        // while (!lock_block())
        // {
        //     moveBlockDown();
        // }
        break;
    default:
        break;
    }
}

void Tetris::moveBlockLeft()
{
    if (!gameOver)
    {
        currBlock.move(0, -1);
        if (is_block_outside() || !is_block_fit())
        {
            currBlock.move(0, 1);

        }
    }
}

void Tetris::moveBlockRight()
{
    if (!gameOver)
    {
        currBlock.move(0, 1);
        if (is_block_outside() || !is_block_fit())
        {
            currBlock.move(0, -1);
        }
    }
}
void Tetris::moveBlockDown()
{
    if (!gameOver)
    {
        currBlock.move(1, 0);
        if (is_block_outside() || !is_block_fit())
        {
            currBlock.move(-1, 0);
            lock_block();//////chặn block lại ko cho dịch qua trái phải khi chạm đáy
        }
    }
}

void Tetris::Draw(RenderWindow& window)
{
    box.draw_cell(window);

    currBlock.draw(window);
}

bool Tetris::is_block_outside()
{
    vector <Position > pos = currBlock.get_curr_position();
    for (Position po : pos)
    {
        if (box.is_cell_outside_box(po.row, po.col))
        {
            return 1;
        }
    }
    return 0;
}
void Tetris::rotate_block()
{

    currBlock.rotate_block();
    if (is_block_outside() || !is_block_fit())
    {
        currBlock.rotate_block();
        currBlock.rotate_block();
        currBlock.rotate_block();
    }
}

void Tetris::lock_block()
{
    vector <Position> pos = currBlock.get_curr_position();

    for (Position po : pos)
    {
        box.box[po.row][po.col] = currBlock.id;
    }
    currBlock = nextBlock;
    if (!is_block_fit())
    {
        currBlock.id = 7;
        //set lại id block current  vì end game rồi và mình muốn chuyển cả block hiện tại này có giá trị là xám grey có id là 7
        game_over();
    }
    nextBlock = randomBlock();
    box.update_box();
}

bool Tetris::is_block_fit()
{
    vector <Position>pos = currBlock.get_curr_position();
    for (Position po : pos)
    {
        if (box.is_empty_cell(po.row, po.col) == false)
        {
            return false;
        }
    }
    return true;
}


void Tetris::game_over()
{
    gameOver = true;
    box.game_over();
}



void Tetris::restart_game()
{
    box.innit();
    blocks = get_all_type_block();
    currBlock = randomBlock();
    nextBlock = randomBlock();
}
