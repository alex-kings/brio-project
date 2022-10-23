#include <iostream>
#include <json/json.h>

int main() {
    Json::Value root;
    std::cout << "Hello\n" << std::flush;

    for(int i = 0; i < 1000; i++){
        std::cout << i;
    }

    return 0;
}
