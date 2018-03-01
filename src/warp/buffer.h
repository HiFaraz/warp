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

  class const_buffer {

    public:
      char const * const  data_;
      std::size_t         size_;

      const_buffer(const char * const data, std::size_t size)
       : data_(data), size_(size) {}
      auto slice(std::size_t from, std::size_t to) const;
      auto to_string() const;
  };

  auto const_buffer::slice(std::size_t from, std::size_t to) const {
    return const_buffer{data_ + from, to - from};
  }

  auto const_buffer::to_string() const {
    return std::string(data_, size_);
  }

  class source_buffer {

    using buffer_t = std::vector<char>;
    
    public:
      source_buffer(std::size_t capacity)
      : data_(buffer_t(capacity)) {
        clear();
      };

      void append(const char* input, int num_bytes = -1);
      void append(const std::string input);
      auto capacity() const;
      void clear();
      auto cursor(); // get pointer to end of data
      auto data();
      void resize(std::size_t new_size);
      auto size() const;
      auto slice(std::size_t from, std::size_t to) const;
      auto to_string() const;

      void inspect() const;

    private:
      buffer_t data_;
      std::size_t size_ = 0;
  };

  auto source_buffer::capacity() const {
    return data_.capacity();
  }

  void source_buffer::clear() {
    // data_.clear();
    size_ = 0;
  }

  auto source_buffer::data() {
    return data_.data();
    // return &data_[0];
  }

  auto source_buffer::size() const {
    // return data_.size();
    return size_;
  }

  auto source_buffer::cursor() {
    return data() + size_;
  }

  auto source_buffer::slice(std::size_t from, std::size_t to) const {
    return const_buffer{&data_[0] + from, to - from};
  }

  auto source_buffer::to_string() const {
    return std::string(data_.data(), size());
  }

  void source_buffer::append(const char* input, int num_bytes) {
    auto input_size = num_bytes == -1 ? std::strlen(input) : num_bytes;
    auto free_space = capacity() - size();
    if (free_space < input_size) {
      data_.reserve(size() + input_size);
    }
    auto pos = cursor();
    resize(size() + input_size);
    std::copy_n(input, input_size, pos);
  }

  void source_buffer::append(const std::string input) {
    append(input.c_str());
  }

  void source_buffer::resize(std::size_t new_size) {
    size_ = new_size;
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