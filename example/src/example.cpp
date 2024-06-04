#include "data.h"
#include <iostream>

int main(int argc, char* argv[]) {

    std::cout << "Name: " << zeno::reflect::type_info<char*>().name() << std::endl;
    std::cout << "Hash: " << zeno::reflect::type_info<char*>().hash_code() << std::endl;

    return 0;
}
