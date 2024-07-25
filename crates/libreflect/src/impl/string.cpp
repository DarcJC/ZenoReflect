#include "reflect/container/string"
#include "polyfill.hpp"
#include <cstring>
#include <execution>
#include <string_view>
#include <algorithm>

namespace zeno
{
namespace reflect
{

    simple_string::simple_string()
        : m_length(0)
        , m_is_heap(false)
    {
        m_ss_buffer[0] = '\0';
    }

    simple_string::simple_string(const char* s) : simple_string() {
        if (s) {
            set_data(s, std::strlen(s));
        }
    }

    simple_string::simple_string(std::string_view s) : simple_string() {
        if (!s.empty()) {
            set_data(s.data(), s.size());
        }
    }

    simple_string::simple_string(const simple_string &other) : simple_string() {
        if (!other.is_empty()) {
            set_data(other.data(), other.m_length);
        }
    }

    simple_string::simple_string(simple_string &&other)
        : m_length(other.m_length)
        , m_capacity(other.m_capacity)
        , m_is_heap(other.m_is_heap)
    {
        if (m_is_heap) {
            m_heap_data = other.m_heap_data;
            other.m_heap_data = nullptr;
        } else {
            std::memcpy(m_ss_buffer, other.m_ss_buffer, m_length + 1);
        }
        other.m_length = 0;
        other.m_capacity = SSO_BUFFER_SIZE;
        other.m_is_heap = false;
    }

    simple_string &simple_string::operator=(const simple_string &other) noexcept
    {
        if (this != &other) {
            simple_string(other).swap(*this);
        }
        return *this;
    }

    simple_string &simple_string::operator=(simple_string &&other) noexcept
    {
        if (this != &other) {
            if (m_is_heap) {
                delete[] m_heap_data;
            }
            m_length = other.m_length;
            m_capacity = other.m_capacity;
            m_is_heap = other.m_is_heap;

            if (m_is_heap) {
                m_heap_data = other.m_heap_data;
                other.m_heap_data = nullptr;
            } else {
                std::memcpy(m_ss_buffer, other.m_ss_buffer, m_length + 1);
            }

            other.m_length = 0;
            other.m_capacity = SSO_BUFFER_SIZE;
            other.m_is_heap = false;
        }
        return *this;
    }

    bool simple_string::operator==(const simple_string& other) const noexcept {
        if (m_length != other.m_length) {
            return false;
        }
        return std::memcmp(data(), other.data(), m_length) == 0;
    }

    bool simple_string::operator!=(const simple_string& other) const noexcept {
        return !(*this == other);
    }

    bool simple_string::operator==(const char* other) const noexcept {
        if (nullptr == other) {
             return false;
        }
        size_t len = std::strlen(other);
        if (m_length != len) {
            return false;
        }
        return std::memcmp(data(), other, m_length) == 0;
    }

    bool simple_string::operator!=(const char* other) const noexcept {
        return !(*this == other);
    }

    simple_string::~simple_string()
    {
        if (m_is_heap) {
            delete[] m_heap_data;
        }
    }

    bool simple_string::is_empty() const noexcept
    {
        return 0 == m_length;
    }

    void simple_string::swap(simple_string &other)
    {
        using std::swap;
        if (&other != this) {
            if (m_is_heap && other.m_is_heap) {
                swap(m_heap_data, other.m_heap_data);
            } else if (m_is_heap) {
                char temp[SSO_BUFFER_SIZE + 1];
                std::memcpy(temp, other.m_ss_buffer, SSO_BUFFER_SIZE + 1);
                other.m_heap_data = m_heap_data;
                m_heap_data = nullptr;
                std::memcpy(m_ss_buffer, temp, SSO_BUFFER_SIZE + 1);
                swap(m_is_heap, other.m_is_heap);
            } else if (other.m_is_heap) {
                char temp[SSO_BUFFER_SIZE + 1];
                std::memcpy(temp, m_ss_buffer, SSO_BUFFER_SIZE + 1);
                m_heap_data = other.m_heap_data;
                other.m_heap_data = nullptr;
                std::memcpy(other.m_ss_buffer, temp, SSO_BUFFER_SIZE + 1);
                swap(m_is_heap, other.m_is_heap);
            } else {
                std::swap_ranges(REFLECT_EXECUTION_CAN_VECTORIZATION, m_ss_buffer, m_ss_buffer + SSO_BUFFER_SIZE + 1, other.m_ss_buffer);
            }
            swap(m_capacity, other.m_capacity);
            swap(m_length, other.m_length);
        }
    }

    simple_string::operator std::string_view() noexcept
    {
        return m_is_heap ? std::string_view(m_heap_data, m_length) : std::string_view(m_ss_buffer, m_length);
    }

    simple_string::operator const char*() const noexcept
    {
        return data();
    }

    simple_string::operator char *() noexcept
    {
        return data();
    }

    char *simple_string::data()
    {
        return m_is_heap ? m_heap_data : m_ss_buffer;
    }

    const char* simple_string::data() const {
        return m_is_heap ? m_heap_data : m_ss_buffer;
    }

    const char* simple_string::c_str() const {
        return data();
    }

    size_t simple_string::length() {
        return m_length;
    }

    void simple_string::ensure_capacity(size_t new_length) {
        if (new_length <= SSO_BUFFER_SIZE || new_length <= m_capacity) {
            return;
        }

        if (!m_is_heap) {
            m_is_heap = true;
            m_capacity = new_length;
            char* new_data = new char[m_capacity + 1];
            std::memcpy(new_data, m_ss_buffer, m_length + 1);
            m_heap_data = new_data;
        } else {
            m_capacity = new_length;
            char* new_data = new char[m_capacity + 1];
            std::memcpy(new_data, m_heap_data, m_length + 1);
            delete[] m_heap_data;
            m_heap_data = new_data;
        }
    }

    simple_string& simple_string::append(const char* s) {
        if (s) {
            size_t new_length = m_length + std::strlen(s);
            ensure_capacity(new_length);
            std::memcpy(data() + m_length, s, std::strlen(s) + 1);
            m_length = new_length;
        }
        return *this;
    }

    simple_string& simple_string::append(const std::string_view s) {
        if (!s.empty()) {
            size_t new_length = m_length + s.size();
            ensure_capacity(new_length);
            std::memcpy(data() + m_length, s.data(), s.size());
            m_length = new_length;
            data()[m_length] = '\0';
        }
        return *this;
    }

    simple_string& simple_string::operator+=(const char* s) {
        return append(s);
    }

    simple_string& simple_string::operator+=(std::string_view s) {
        return append(s);
    }

    simple_string& simple_string::operator+=(const simple_string& other) {
        return append(const_cast<simple_string&>(other).operator std::string_view());
    }

    void simple_string::set_data(const char* s, size_t len) {
        if (len <= SSO_BUFFER_SIZE) {
            if (m_is_heap) {
                delete[] m_heap_data;
                m_is_heap = false;
            }
            std::memcpy(m_ss_buffer, s, len);
            m_ss_buffer[len] = '\0';
            m_length = len;
            m_capacity = SSO_BUFFER_SIZE;
        } else {
            ensure_capacity(len);
            std::memcpy(data(), s, len);
            data()[len] = '\0';
            m_length = len;
        }
    }

}
}
