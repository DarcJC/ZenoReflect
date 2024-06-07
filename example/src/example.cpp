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

        {
            const Yeppp pp{};
            Any any(pp);
            std::cout << "Any: " << any.type().name() << std::endl;
        }

        {
            Any any = make_any<Yeppp>(Yeppp {
                .field1 = 24566,
                .field2 = "aaaaa", 
                .field3 = false,
            });
            std::cout << "Any: " << any.type().name() << std::endl;

            Yeppp* wow = any_cast<Yeppp>(&any);
            std::cout << "field1: " << wow->field1 << "\tfield2: " << wow->field2 << "\tfield3: " << wow->field3 << std::endl;
            wow->field3 = true;
            wow->field1 = 456;
            wow = any_cast<Yeppp>(&any);

            const Yeppp& qwq = any_cast<Yeppp>(any);
            std::cout << "field1: " << qwq.field1 << "\tfield2: " << qwq.field2 << "\tfield3: " << qwq.field3 << std::endl;

            Yeppp& lol = any_cast<Yeppp&>(any);
            {
                Yeppp tmp{.field1 = 12313};
                lol = tmp; // Copy!
            }
            std::cout << "field1: " << qwq.field1 << "\tfield2: " << qwq.field2 << "\tfield3: " << qwq.field3 << std::endl;
        }
    }

    return 0;
}
