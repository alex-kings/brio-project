#pragma once

class Adder {
    public:
    Adder(float a, float b);
    float getResult();

    private:
    void doAddition(float a, float b);
    float result = 0;
};