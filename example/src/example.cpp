#include "data.h"
#include "reflect/container/object_proxy"
#include "reflect/container/any"
#include "reflect/container/arraylist"
#include <iostream>


void zeno::IAmPrimitve::DoSomething(int value) {
    std::cout << "Doing something " << value << "x ..." << std::endl;
}

int main(int argc, char* argv[]) {

#if 0
    std::cout << "Name: " << zeno::reflect::type_info<char*>().name() << std::endl;
    std::cout << "Hash: " << zeno::reflect::type_info<char*>().hash_code() << std::endl;

    {
        using namespace zeno::reflect;
        {
            IAmPrimitve p{};
            p.i32 = 10086;
            MemberProxy proxy(&IAmPrimitve::i32);
            std::cout << "Proxied Field: " << proxy(p) << std::endl;
            MemberProxy proxy2(&IAmPrimitve::DoSomething);
            proxy2(p, p.i32);
        }

        {
            const Yeppp pp{};
            Any any(pp);
            std::cout << "Any: " << any.type().name() << std::endl;
        }

        {
            Any any = make_any<Yeppp>(Yeppp {
                24566,
                "aaaaa", 
                false,
            });
            std::cout << "Any: " << any.type().name() << std::endl;

            Yeppp* wow = any_cast<Yeppp>(&any);
            std::cout << "field1: " << wow->field1 << "\tfield2: " << wow->field2 << "\tfield3: " << wow->field3 << std::endl;
            wow->field3 = true;
            wow->field1 = 456;
            wow = any_cast<Yeppp>(&any);

            const Yeppp& qwq = any_cast<Yeppp&>(any);
            std::cout << "field1: " << qwq.field1 << "\tfield2: " << qwq.field2 << "\tfield3: " << qwq.field3 << std::endl;

            Yeppp& lol = any_cast<Yeppp&>(any);
            std::cout << "Address of lol: " << &lol << "\tAddress of wow: " << wow << "\tAddress of qwq: " << &qwq << std::endl;
            {
                Yeppp tmp{12313};
                lol = tmp; // Copy!
            }
            std::cout << "field1: " << qwq.field1 << "\tfield2: " << qwq.field2 << "\tfield3: " << qwq.field3 << std::endl;
        }

        {
            ArrayList<Yeppp> arr{};
            arr.add_item({});
        }
    }
#endif

    return 0;
}
