#pragma once
#include "Vec2D.h"

class Connector {
private:
    Vec2D position;
    bool type; // true = out, false = in.

public:
    Connector(float x, float y, bool connector_type) {
        position = Vec2D(x, y);
    }
};
