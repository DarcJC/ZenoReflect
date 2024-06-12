#include "template_literal"

using namespace zeno::reflect::text;

static const char* RTTI =
    #include "RTTI.inja"
;

static const char* GENERATED_TEMPLATE_HEADER_TEMPLATE =
    #include "generated_template_header.inja"
;
