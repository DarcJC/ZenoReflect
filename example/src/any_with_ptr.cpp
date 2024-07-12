#include "data.h"
#include "test.h"
#include "reflect/type"
#include "reflect/container/any"
#include "reflect/reflection.generated.hpp"

using namespace zeno::reflect;

int main() {

    Any soo = make_any<Soo>();
    Any param1 = make_any<int>(123);

    TypeHandle ttt = get_type<Soo>();

    ttt->get_member_functions()[0]->invoke(soo, { param1 });

    return 0;
}
