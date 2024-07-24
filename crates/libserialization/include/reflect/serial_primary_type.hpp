#pragma once

#include "reflect/traits/constant_eval.hpp"
#include <string>
#include <cstdint>

namespace zeno
{
namespace reflect
{

template <typename T>
struct IsSerializationPrimaryType : TFalseType {};

template<> struct IsSerializationPrimaryType<float> : TTrueType {};
template<> struct IsSerializationPrimaryType<double> : TTrueType {};
template<> struct IsSerializationPrimaryType<char> : TTrueType {};
template<> struct IsSerializationPrimaryType<std::string> : TTrueType {};
template<> struct IsSerializationPrimaryType<int8_t> : TTrueType {};
template<> struct IsSerializationPrimaryType<int16_t> : TTrueType {};
template<> struct IsSerializationPrimaryType<int32_t> : TTrueType {};
template<> struct IsSerializationPrimaryType<int64_t> : TTrueType {};
template<> struct IsSerializationPrimaryType<uint8_t> : TTrueType {};
template<> struct IsSerializationPrimaryType<uint16_t> : TTrueType {};
template<> struct IsSerializationPrimaryType<uint32_t> : TTrueType {};
template<> struct IsSerializationPrimaryType<uint64_t> : TTrueType {};

}
}
