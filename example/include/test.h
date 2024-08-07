#include "data.h"
#include "reflect/container/any"
#include "reflect/core.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include "reflect/reflection.generated.hpp"

namespace zeno {

    class ZRECORD() ABC {
    public:
        ABC() = delete;
    };

}

using ZABC = zeno::ABC;

struct ZRECORD() Soo : public zeno::reflect::any::TEnableAnyFromThis<Soo> {

    ZMETHOD(DisplayName = "123")
    void wow(int* qwq) {
        std::cout << (size_t)qwq << std::endl;
    }
};
