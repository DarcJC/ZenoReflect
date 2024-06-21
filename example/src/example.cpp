#include "data.h"
#include "reflect/container/object_proxy"
#include "reflect/container/any"
#include "reflect/container/arraylist"
#include <iostream>
#include <functional>

using namespace zeno::reflect;

void zeno::IAmPrimitve::DoSomething(int value) const {
    std::cout << "Doing something " << value << "x ..." << std::endl;
}

namespace zeno {
    std::ostream& operator<<(std::ostream& os, const IAmPrimitve& p) {
        os << "i32 = " << p.i32;
        return os;
    }

namespace reflect {

    std::ostream& operator<<(std::ostream& os, const Any& any) {
        // 检查any容器是否为空 也可以用any.has_value()
        if (any) {
            // 获取类型句柄 这玩意大小只有 aligned(sizeof(size_t) + sizeof(bool), sizeof(size_t)) 可以随意拷贝
            TypeHandle type_visitor = any.type();
            // 打印信息
            os << "===\t" << type_visitor->get_info().canonical_typename.c_str() << "\t===\n";
            // 获取类型所有字段的访问器
            const auto& fields =  type_visitor->get_member_fields();
            // 遍历类型字段
            for (const auto& field_visitor : fields) {
                // 打印字段名称
                os << "\t" << field_visitor->get_name().c_str() << "\t=\t";
                // 各种类型的输出适配器 这里只实现了int的
                if (field_visitor->get_field_type() == get_type<int>()) {
                    os << *field_visitor->get_field_ptr_typed<int>(any) << "\n";
                }
            }
            os << "===========================================";
        } else {
            os << "<nullany>";
        }
        return os;
    }
}
}

void fix_console_encoding();

#ifdef _MSC_VER
#include <windows.h>

void fix_console_encoding() {
    SetConsoleOutputCP(CP_UTF8);
    std::locale::global(std::locale("en_US.UTF-8"));
}

#else
void fix_console_encoding() {}
#endif

int main(int argc, char* argv[]) {
    fix_console_encoding();

    // 获取类型名称
    TypeHandle handle = get_type<zeno::IAmPrimitve>();
    std::cout << "类型名称: " << handle->get_info().canonical_typename.c_str() << std::endl;

    zeno::IAmPrimitve hand_made_inst{};
    hand_made_inst.i32 = 789;
    std::cout << "直接构造: " << hand_made_inst << std::endl;

    TypeBase* type = handle.get_reflected_type_or_null();
    ITypeConstructor& ctor = type->get_constructor_checked({ zeno::reflect::type_info<int>() });

    std::cout << "这是调用构造函数所需的参数: ";
    for (const auto& t : ctor.get_params()) {
        std::cout << t.name() << "  ";
    }
    std::cout << std::endl;

    zeno::IAmPrimitve reflect_inst = ctor.create_instance_typed<zeno::IAmPrimitve>({ ctor.get_param_default_value(0) });
    std::cout << "使用反射调用拷贝构造函数创建的新实例: " << reflect_inst << std::endl;
    reflect_inst.i32 = 123;
    std::cout << "可以像正常对象一样访问: " << reflect_inst << std::endl;

    Any type_erased_inst = ctor.create_instance({ Any(678), ctor.get_param_default_value(1) });
    std::cout << "基于反射信息输出对象: \n" << type_erased_inst << std::endl;

    // 或者也可以通过直接赋值给Any 也支持移动语义
    Any use_operator_equal = hand_made_inst;
    std::cout << "直接赋值构造Any: \n" << use_operator_equal << std::endl;

    // 输出所有有DisplayName的反射类型
    {
        auto& registry = zeno::reflect::ReflectionRegistry::get();
        for (const auto& type : registry->all()) {
            if (const IRawMetadata * metadata = type->get_metadata()) {
                if (const IMetadataValue* value = metadata->get_value("DisplayName")) {
                    std::cout << "反射类型 " << type->get_info().canonical_typename.c_str() << " 的 DisplayName = \"" << value->as_string() << "\"" << std::endl;
                }
            }
        }
    }

    return 0;
}
