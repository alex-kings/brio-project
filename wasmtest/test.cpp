#include "test.h"

Adder::Adder(float a, float b) {
    this->doAddition(a,b);
}

void Adder::doAddition(float a, float b) {
    this->result = a + b;
}

float Adder::getResult() {
    return this->result;
}