#include "data.h"
#include "reflect/container/object_proxy"
#include "reflect/container/any"
#include "reflect/container/arraylist"
#include <iostream>
#include <functional>

using namespace zeno::reflect;

void zeno::IAmPrimitve::DoSomething(int value) {
    std::cout << "Doing something " << value << "x ..." << std::endl;
}

namespace zeno {
    std::ostream& operator<<(std::ostream& os, const IAmPrimitve& p) {
        os << "i32 = " << p.i32;
        return os;
    }
}

int main(int argc, char* argv[]) {

    TypeHandle handle = get_type<zeno::IAmPrimitve>();
    std::cout << "Type name: " << handle->get_info().canonical_typename.c_str() << std::endl;

    zeno::IAmPrimitve hand_made_inst{};
    hand_made_inst.i32 = 456;
    std::cout << "Hand Made: " << hand_made_inst << std::endl;

    TypeBase* type = handle.get_reflected_type_or_null();
    ITypeConstructor* ctor = type->get_constructor({ type_info<zeno::IAmPrimitve>() })[0];

    std::cout << "Args to invoke ctor: ";
    for (const auto& t : ctor->get_params()) {
        std::cout << t.name() << "  ";
    }
    std::cout << std::endl;

    zeno::IAmPrimitve reflect_inst = ctor->create_instance_typed<zeno::IAmPrimitve>({ Any(hand_made_inst) });
    std::cout << "Reflection copied: " << reflect_inst << std::endl;
    reflect_inst.i32 = 123;
    std::cout << "Modified copied: " << reflect_inst << std::endl;

    return 0;
}
