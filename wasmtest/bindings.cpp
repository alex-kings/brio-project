#include <emscripten/bind.h>
#include "test.h"

using namespace emscripten;


EMSCRIPTEN_BINDINGS(Adder) {
   class_<Adder>("Adder")
      .constructor<float, float>()
      .function("getResult", &Adder::getResult)
      ;
}