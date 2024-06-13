#include "data.h"
#include "reflect/container/object_proxy"
#include "reflect/container/any"
#include "reflect/container/arraylist"
#include <iostream>


void zeno::IAmPrimitve::DoSomething(int value) {
    std::cout << "Doing something " << value << "x ..." << std::endl;
}

int main(int argc, char* argv[]) {

    return 0;
}
