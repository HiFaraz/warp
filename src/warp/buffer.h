#ifndef WARP_BUFFER_H
#define WARP_BUFFER_H

#include <algorithm> // std::copy_n
#include <cstddef> // std::size_t
#include <cstring> // std::strlen
#include <iostream> // std::ostream
#include <stdexcept> // std::out_of_range
#include <string> // std::string
#include <vector> // std::vector

namespace warp {

  class slice_buffer {

    public:
      slice_buffer() {}
      slice_buffer(char const * data, std::size_t size)
       : data_(data), size_(size) {}
      auto slice(std::size_t from, std::size_t to) const;
      auto to_string() const;

      char const * data_;
      std::size_t   size_;
  };

  auto slice_buffer::slice(std::size_t from, std::size_t to) const {
    return slice_buffer{data_ + from, to - from};
  }

  auto slice_buffer::to_string() const {
    return std::string(data_, size_);
  }

  class source_buffer {

    using buffer_t = std::vector<char>;
    
    public:
      source_buffer(std::size_t capacity)
      : data_(buffer_t(capacity)) {
        clear();
      };

      // modify the buffer
      void append(const char* input, std::size_t length = -1);
      void append(const std::string input);
      void append(source_buffer& buffer);
      void clear();
      void expand(std::size_t new_capacity);
      void resize(std::size_t new_size);

      // inspect the buffer
      auto begin();
      auto capacity() const;
      auto end();
      auto remaining() const;
      auto size() const;
      auto to_string() const;

      // extract from the buffer
      auto slice(std::size_t from, std::size_t to) const;

      void inspect() const;

    private:
      buffer_t    data_;
      std::size_t size_ = 0;
  };

  auto source_buffer::capacity() const {
    return data_.capacity();
  }

  void source_buffer::clear() {
    resize(0);
  }

  auto source_buffer::begin() {
    return data_.data();
  }

  auto source_buffer::size() const {
    return size_;
  }

  auto source_buffer::end() {
    return begin() + size_;
  }

  auto source_buffer::slice(std::size_t from, std::size_t to) const {
    return slice_buffer{&data_[0] + from, to - from};
  }

  auto source_buffer::to_string() const {
    return std::string(data_.data(), size());
  }

  void source_buffer::append(const char* input, std::size_t length) {
    auto input_size = length == -1 ? std::strlen(input) : length;
    if (size() + input_size > capacity()) {
      data_.reserve(size() + input_size);
    }
    auto pos = end();
    resize(size() + input_size);
    std::copy_n(input, input_size, pos);
  }

  void source_buffer::append(const std::string input) {
    append(input.c_str());
  }

  void source_buffer::append(source_buffer& buffer) {
    append(buffer.begin(), buffer.size());
  }

  void source_buffer::expand(std::size_t new_capacity) {
    if (new_capacity > capacity()) {
      data_.reserve(new_capacity);
    }
  }

  auto source_buffer::remaining() const {
    return capacity() - size();
  }

  void source_buffer::resize(std::size_t new_size) {
    size_ = new_size;
    expand(size_);
  }

  void source_buffer::inspect() const {
    std::cout << "Buffer inspect: {" << std::endl;
    std::cout << "  capacity(): " << capacity() << std::endl;
    std::cout << "  data(): " << data_.data() << std::endl;
    std::cout << "  size(): " << size() << std::endl;
    std::cout << "  to_string(): " << to_string() << std::endl;
    std::cout << "}" << std::endl;
  }

  // std::ostream& operator<<(std::ostream& stream, source_buffer& buffer) {  
  //   stream << buffer.to_string();
  //   return stream;
  // }

}

#endif // !WARP_BUFFER_H