#include "data.h"
#include "reflect/container/object_proxy"
#include <iostream>

int main(int argc, char* argv[]) {

    std::cout << "Name: " << zeno::reflect::type_info<char*>().name() << std::endl;
    std::cout << "Hash: " << zeno::reflect::type_info<char*>().hash_code() << std::endl;

    {
        using namespace zeno::reflect;
        IAmPrimitve p{.i32 = 10086};
        MemberProxy proxy(&IAmPrimitve::i32);
        std::cout << "Proxied: " << proxy(p) << std::endl;
    }

    return 0;
}
