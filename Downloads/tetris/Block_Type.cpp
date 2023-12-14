#pragma once

#include "Block.h"
#include "Position.h"

class L_block : public Block
{
public:
    L_block()
    {
        this->id = 0;
        Block();
        block[0] = { Position(0,2),Position(1,0),Position(1,1),Position(1,2) };
        block[1] = { Position(0,1),Position(1,1),Position(2,1),Position(2,2) };
        block[2] = { Position(1,0),Position(1,1),Position(1,2),Position(2,0) };
        block[3] = { Position(0,0),Position(0,1),Position(1,1),Position(2,1) };
        // dịch ra giữa màn hình để block rơi xuống gây trung tâm lúc đầu khi xuất hiện)
        move(0, 3);
    }

};
class J_block : public Block
{
public:
    J_block()
    {
        this->id = 1;
        Block();
        block[0] = { Position(0,0),Position(1,0),Position(1,1),Position(1,2) };
        block[1] = { Position(0,1),Position(0,2),Position(1,1),Position(2,1) };
        block[2] = { Position(1,0),Position(1,1),Position(1,2),Position(2,2) };
        block[3] = { Position(0,1),Position(1,1),Position(2,0),Position(2,1) };
        move(0, 3);

    }

};
class I_block : public Block
{
public:
    I_block()
    {
        this->id = 2;
        Block();
        block[0] = { Position(1,0),Position(1,1),Position(1,2),Position(1,3) };
        block[1] = { Position(0,2),Position(1,2),Position(2,2),Position(3,2) };
        block[2] = { Position(2,0),Position(2,1),Position(2,2),Position(2,3) };
        block[3] = { Position(0,1),Position(1,1),Position(2,1),Position(3,1) };
        move(-1, 3);

    }
};
class O_block : public Block
{
public:
    O_block()
    {
        this->id = 3;
        Block();
        block[0] = { Position(0,0),Position(0,1),Position(1,0),Position(1,1) };
        block[1] = { Position(0,0),Position(0,1),Position(1,0),Position(1,1) };
        block[2] = { Position(0,0),Position(0,1),Position(1,0),Position(1,1) };
        block[3] = { Position(0,0),Position(0,1),Position(1,0),Position(1,1) };
        move(0, 3);

    }
};
class S_block : public Block
{
public:
    S_block()
    {
        this->id = 4;
        Block();
        block[0] = { Position(0,1),Position(0,2),Position(1,0),Position(1,1) };
        block[1] = { Position(0,1),Position(1,1),Position(1,2),Position(2,2) };
        block[2] = { Position(1,1),Position(1,2),Position(2,0),Position(2,1) };
        block[3] = { Position(0,0),Position(1,0),Position(1,1),Position(2,1) };
        move(0, 3);


    }
};

class T_block : public Block
{
public:
    T_block()
    {
        this->id = 5;
        Block();
        block[0] = { Position(0,1),Position(1,0),Position(1,1),Position(1,2) };
        block[1] = { Position(0,1),Position(1,1),Position(1,2),Position(2,1) };
        block[2] = { Position(1,0),Position(1,1),Position(1,2),Position(2,1) };
        block[3] = { Position(0,1),Position(1,0),Position(1,1),Position(2,1) };
        move(0, 3);

    }
};

class Z_block : public Block
{
public:
    Z_block()
    {
        this->id = 6;
        Block();
        block[0] = { Position(0,0),Position(0,1),Position(1,1),Position(1,2) };
        block[1] = { Position(0,2),Position(1,1),Position(1,2),Position(2,1) };
        block[2] = { Position(1,0),Position(1,1),Position(2,1),Position(2,2) };
        block[3] = { Position(0,1),Position(1,0),Position(1,1),Position(2,0) };
        move(0, 3);

    }
};