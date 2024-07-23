#pragma once

#include <iostream>
#include "args.hpp"

#define ZENO_REFLECTION_LOG_DEBUG(...) \
    if (nullptr != GLOBAL_CONTROL_FLAGS && GLOBAL_CONTROL_FLAGS->verbose) {\
        std::cout << __VA_ARGS__ << << std::endl;\
    }
