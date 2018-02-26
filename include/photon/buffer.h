#ifndef PHOTON_BUFFER_H
#define PHOTON_BUFFER_H

#include <cstddef> // size_t
#include <cstring> // memset
#include <iostream> // std::ostream

class Buffer {
  
  public:
    char* data;
    std::size_t capacity;
    std::size_t size;

    Buffer(std::size_t capacity) {
      data = new char[capacity];
      this->capacity = capacity;
      this->size = capacity;
    }

    Buffer (std::size_t size, int filler) : Buffer(size) {
      fill(filler);
    }

    void fill(int filler) {
      memset(data, filler, size);
    }

    char operator[] (std::size_t index) {
      return data[index];
    }
};

std::ostream& operator<<(std::ostream& os, const Buffer& buffer) {  
    os << buffer.data;
    return os;  
}  

#endif // !PHOTON_BUFFER_H