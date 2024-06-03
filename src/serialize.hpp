#pragma once

#include <limits>
#include "bitsery/bitsery.h"
#include "bitsery/adapter/buffer.h"
#include "bitsery/traits/vector.h"
#include "bitsery/traits/string.h"
#include "bitsery/brief_syntax/unordered_map.h"
#include "bitsery/brief_syntax.h"
#include "bitsery/ext/std_map.h"
#include "bitsery/ext/std_tuple.h"
#include "bitsery/ext/std_variant.h"
#include "bitsery/ext/compact_value.h"
#include "metadata.hpp"

namespace zeno
{
namespace reflect 
{
    using ByteBuffer = std::vector<uint8_t>;
    using OutputAdapter = bitsery::OutputBufferAdapter<ByteBuffer>;
    using InputAdapter = bitsery::InputBufferAdapter<ByteBuffer>;
}
}

namespace bitsery {

constexpr uint32_t MAX_NUM = std::numeric_limits<uint32_t>::max();

}

