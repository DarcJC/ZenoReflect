#include <string>
#include <vector>
#include "reflect/container/any"
#include "reflect/core.hpp"
#include "reflect/reflection.generated.hpp"
#include "reflect/registry.hpp"
#include "reflect/serialization.hpp"

struct ZRECORD() HowAreYou : public zeno::reflect::any::TEnableAnyFromThis<HowAreYou> {
    int32_t a = 114514;
    float b = 3.1415926;
    std::string c = "啊？？？";
    uint8_t d = 254;

    bool serialize(zeno::reflect::Archive& archive) {
        archive.archive(a);
        archive.archive(b);
        archive.archive(c);
        archive.archive(d);

        return true;
    }
};
