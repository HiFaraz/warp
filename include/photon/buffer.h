#ifndef PHOTON_BUFFER_H
#define PHOTON_BUFFER_H

#include <cstddef> // size_t
#include <cstring> // memcpy, memset
#include <iostream> // std::ostream
#include <stdexcept>

class Buffer {
  
  public:
    char* data;
    std::size_t capacity;
    std::size_t size;

    Buffer(std::size_t capacity) {
      data = new char[capacity];
      this->capacity = capacity;
      size = 0;
    }

    Buffer (std::size_t size, int filler) : Buffer(size) {
      fill(filler);
    }

    void append(const char* input) {
      size_t input_size = strlen(input);
      size_t free_space = capacity - size;
      if (free_space < input_size) {
        throw std::runtime_error{"Not enough space in Buffer, flush the buffer first"};
      }
      memcpy(data + size, input, input_size);
      size += input_size;
    }

    void empty() {
      size = 0;
    }

    void fill(int filler) {
      memset(data, filler, size);
    }

    char operator[] (std::size_t index) {
      return data[index];
    }

    Buffer& operator<<(const char* input) {
      append(input);
      return *this;
    }
};

std::ostream& operator<<(std::ostream& os, const Buffer& buffer) {  
  os << buffer.data;
  return os;  
}

#endif // !PHOTON_BUFFER_H