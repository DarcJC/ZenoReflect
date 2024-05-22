#include "args.hpp"
#include "utils.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {
    ControlFlags flags = parse_args(argc, argv);
    GLOBAL_CONTROL_FLAGS = &flags;
    if (flags.verbose) {
        flags.print();
    }

    int32_t result = 0;
    for (const std::string& filepath : flags.input_sources) {
        std::optional<std::string> source_str = read_file(filepath);
        if (!source_str.has_value()) {
            std::cerr << fmt::format("Can't read source file {}", filepath) << std::endl;
            return 2;
        }
        std::string source = source_str.value();

        // result += traverse_file_ast({
        //     .identity_name = filepath,
        //     .source = source,
        //     .type = TranslationUnitType::Header,
        // });
    }
    return result;
}
