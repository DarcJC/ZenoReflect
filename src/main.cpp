#include "args.hpp"
#include "utils.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {
    ControlFlags flags = parse_args(argc, argv);
    GLOBAL_CONTROL_FLAGS = &flags;
    if (GLOBAL_CONTROL_FLAGS->verbose) {
        GLOBAL_CONTROL_FLAGS->print();
    }

    int32_t result = 0;
    for (const std::string& filepath : GLOBAL_CONTROL_FLAGS->input_sources) {
        std::optional<std::string> source_str = read_file(filepath);
        if (!source_str.has_value()) {
            std::cerr << fmt::format("Can't read source file {}", filepath) << std::endl;
            return 2;
        }
        std::string source = source_str.value();

        ReflectionModel model{};
        result += static_cast<int32_t>(generate_reflection_model({
            .identity_name = filepath,
            .source = source,
            .type = TranslationUnitType::Header,
        }, model));
    }
    return result;
}
