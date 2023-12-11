#pragma once


#include "Headers/Block.h"
#include "Headers/Position.h"

class L_block : public Block
{
public:
    L_block()
    {
        this -> id  =  0;
        Block ();
        block[0] = {Position(0,2),Position(1,0),Position(1,1),Position(1,2)};
        block[1] = {Position(0,1),Position(1,1),Position(2,1),Position(2,2)};
        block[2] = {Position(1,0),Position(1,1),Position(1,2),Position(2,0)};
        block[3] = {Position(0,0),Position(0,1),Position(1,1),Position(2,1)};
    }

};