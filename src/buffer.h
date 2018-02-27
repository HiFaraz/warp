#ifndef WARP_BUFFER_H
#define WARP_BUFFER_H

#include <cstddef> // std::size_t
#include <cstring> // memcpy, memset
#include <iostream> // std::ostream
#include <stdexcept>
#include <string> // std::string

class Buffer {
  
  public:
    char* data;
    std::size_t capacity;
    std::size_t size;

    Buffer(std::size_t capacity) : capacity(capacity) {
      data = new char[capacity];
      size = 0;
    };

    Buffer (std::size_t size, int filler) : Buffer(size) {
      fill(filler);
    }

    void append(const char* input);
    void append(const std::string input);
    void append(const Buffer& buffer);
    void empty();
    void fill(const int filler);
    char operator[](const std::size_t index);

    template <typename T>
    Buffer& operator<<(const T input);
};

void Buffer::append(const char* input) {
  size_t input_size = strlen(input);
  size_t free_space = capacity - size;
  if (free_space < input_size) {
    throw std::runtime_error{"Not enough space in Buffer, flush the buffer first"};
  }
  memcpy(data + size, input, input_size);
  size += input_size;
}

void Buffer::append(const std::string input) {
  append(input.c_str());
}

void Buffer::append(const Buffer& buffer) {
  append(buffer.data);
}

void Buffer::empty() {
  size = 0;
}

void Buffer::fill(const int filler) {
  memset(data, filler, size);
}

char Buffer::operator[](const std::size_t index) {
  return data[index];
}

template <typename T>
Buffer& Buffer::operator<<(const T input) {
  append(input);
  return *this;
}

std::ostream& operator<<(std::ostream& stream, Buffer& buffer) {  
  stream << buffer.data;
  return stream;
}

#endif // !WARP_BUFFER_H