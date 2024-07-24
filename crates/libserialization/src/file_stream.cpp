#include "reflect/container/unique_ptr"
#include "reflect/serialization.hpp"
#include <fstream>
#include <cstdint>

namespace zeno
{
namespace reflect
{

class FileReadableStream : public IReadableStream {
public:
    explicit FileReadableStream(std::string_view file_path) : input_stream(std::string(file_path), std::ios::binary | std::ios::in) {
        if (!input_stream) {
            throw std::runtime_error("Failed to open file for reading");
        }
    }

    void read(uint8_t* buf, size_t len) override {
        input_stream.read(reinterpret_cast<char*>(buf), len);
        if (!input_stream) {
            throw std::runtime_error("Failed to read from file");
        }
    }

    uint8_t next_byte() override {
        char byte;
        input_stream.get(byte);
        if (!input_stream) {
            throw std::runtime_error("Failed to read next byte from file");
        }
        return static_cast<uint8_t>(byte);
    }

    bool is_valid() override {
        return input_stream.good() && input_stream.is_open();
    }

private:
    mutable std::ifstream input_stream;
};

class FileWritableStream : public IWritableStream {
public:
    explicit FileWritableStream(std::string_view file_path) : output_stream(std::string(file_path), std::ios::binary | std::ios::out) {
        if (!output_stream) {
            throw std::runtime_error("Failed to open file for writing");
        }
    }

    void write_byte(uint8_t data) override {
        output_stream.put(static_cast<char>(data));
        if (!output_stream) {
            throw std::runtime_error("Failed to write byte to file");
        }
    }

    void write(const uint8_t* data, size_t len) override {
        output_stream.write(reinterpret_cast<const char*>(data), len);
        if (!output_stream) {
            throw std::runtime_error("Failed to write data to file");
        }
    }

    bool is_valid() override {
        return output_stream.good() && output_stream.is_open();
    }

private:
    std::ofstream output_stream;
};

UniquePtr<IReadableStream> create_file_read_stream(std::string_view path) {
    return make_unique<FileReadableStream>(path);
}

UniquePtr<IWritableStream> create_file_write_stream(std::string_view path) {
    return make_unique<FileWritableStream>(path);
}

} // namespace reflect
} // namespace zeno
