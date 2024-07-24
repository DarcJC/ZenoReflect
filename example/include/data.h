#pragma once

#include "reflect/core.hpp"
#include "reflect/type"
#include "reflect/reflection_traits.hpp"
#include <limits>
#include <string>
#include <vector>
#include <iostream>
#include "reflect/reflection.generated.hpp"

namespace test {
struct Parent {
    int abc;
};
namespace inner {
struct Oops {
    int field1;
    const char* field2 = "abc";
    bool field3 = true;
};
union Yeppp {};
}
}

using AliasType1 = test::Parent;
typedef test::Parent AliasType2;
using Yeppp = test::inner::Oops;


namespace zeno
{
    struct ZRECORD(����1="��", �����=("��1", "Group 2", "Group 3"), a123="true", DisplayName="����һ��Prim") IAmPrimitve {
        IAmPrimitve(const IAmPrimitve&) = default;

        signed int i32 = 10086;

        std::string s = "��������";

        ZMETHOD(Name="��Щ��") 
        void DoSomething(int value) const {
            std::cout << "Doing something " << value << "x ..." << std::endl;
        }
    };

    struct ZRECORD(DisplayName="���") ������� : public AliasType2 {
        
        �������() = default;

        ZPROPERTY(DisplayName = "Field 1")
        const char* �ֶ�1 = "Hello World";
    };

    class ZRECORD(Test="true", StrList=("Group 1", "Group 2", "Group 3")) Hhhh : public reflect::TEnableVirtualRefectionInfo<Hhhh> {
    public:
        int a1;

        ZPROPERTY(Role = "input", Control = zeno::LineEdit, IntValue = 233, FloatVal = 0, ComboBoxItems = ("Option A", "Option B", "Option C"), minmax=(2,4))
        std::string test;

        zeno::reflect::Any a2;
    };
}

REFLECT_REGISTER_RTTI_TYPE_MANUAL(std::vector<std::string>)
