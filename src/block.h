#pragma once

struct Block {
    bool solid;
    int type;
    Block() : solid(false), type(0) {}
    Block(bool solid, int type) : solid(solid), type(type) {}
};
