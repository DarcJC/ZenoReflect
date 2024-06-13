#include "data.h"
#include "reflect/container/object_proxy"
#include "reflect/container/any"
#include "reflect/container/arraylist"
#include <iostream>

using namespace zeno::reflect;

void zeno::IAmPrimitve::DoSomething(int value) {
    std::cout << "Doing something " << value << "x ..." << std::endl;
}

int main(int argc, char* argv[]) {

    TypeHandle handle = get_type<zeno::IAmPrimitve>();
    std::cout << "Type name: " << handle->get_info().canonical_typename.c_str() << std::endl;

    return 0;
}
