#include "data.h"
#include "reflect/container/object_proxy"
#include "reflect/container/any"
#include <iostream>

int main(int argc, char* argv[]) {

    std::cout << "Name: " << zeno::reflect::type_info<char*>().name() << std::endl;
    std::cout << "Hash: " << zeno::reflect::type_info<char*>().hash_code() << std::endl;

    {
        using namespace zeno::reflect;
        IAmPrimitve p{.i32 = 10086};
        MemberProxy proxy(&IAmPrimitve::i32);
        std::cout << "Proxied: " << proxy(p) << std::endl;

        const Yeppp pp{};
        Any any(pp);
        std::cout << "Any: " << any.type().name() << std::endl;
    }

    return 0;
}
