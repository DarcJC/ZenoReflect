#include "data.h"
#include "reflect/core.hpp"
#include "reflect/reflection.generated.hpp"

namespace zeno {

    class ABC {
    public:
        ABC() = delete;
    };

}

using ZABC = zeno::ABC;
