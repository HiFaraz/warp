#include <algorithm> // std::copy_n
#include <cassert> // assert
#include <cstring> // memcpy
#include <warp/buffer> // warp::source_buffer

template<typename base, typename T>
auto is_instance_of(T const * const ptr) {
  return dynamic_cast<const base*>(ptr) != nullptr;
}

int main() {
  // initializes with a capacity
  auto source = warp::source_buffer{11};
  assert(source.capacity() == 11);

  // a new buffer is empty
  assert(source.begin() == source.end());
  assert(source.size() == 0);
  assert(source.to_string() == "");

  // can append on empty buffer
  source.append("hello");
  assert(source.size() == 5);
  assert(source.to_string() == "hello");

  // retains capacity on append
  assert(source.capacity() == 11);

  // can append on non-empty buffer
  source.append(" world");
  assert(source.size() == 11);
  assert(source.to_string() == "hello world");
  assert(source.remaining() == 0); // buffer is full

  // can append on full buffer without losing contents
  // allocates new memory and expands the buffer
  source.append("!");
  assert(source.capacity() == 12); // expanded
  assert(source.size() == 12);
  assert(source.remaining() == 0); // buffer is still full
  assert(source.to_string() == "hello world!");

  // can clear a buffer
  source.clear();
  assert(source.begin() == source.end());
  assert(source.size() == 0);
  assert(source.capacity() == 12); // retain capacity
  assert(source.to_string() == "");

  // can append on cleared buffer
  source.append("yoyo");
  assert(source.size() == 4);
  assert(source.to_string() == "yoyo");

  // works with std::copy_n
  std::copy_n("cp", 2, source.begin());
  assert(source.capacity() == 12);
  assert(source.size() == 4);
  assert(source.to_string() == "cpyo");

  // can std::copy_n into the middle
  source.resize(2);
  std::copy_n("cp", 2, source.end());
  // but since we modifed the buffer manually,
  // we must also specify the new size
  source.resize(4);
  assert(source.to_string() == "cpcp");

  // works with memcpy
  memcpy(source.begin(),"mc",2);
  assert(source.capacity() == 12);
  assert(source.size() == 4);
  assert(source.to_string() == "mccp");

  // can memcpy into the middle
  source.resize(2);
  memcpy(source.end(), "mc", 2);
  // but since we modifed the buffer manually,
  // we must also specify the new size
  source.resize(4);
  assert(source.to_string() == "mcmc");

  // can resize the buffer capacity
  // expand: increase capacity (no decrease)
  // resize: modify size up or down, expand if needed
  source.resize(130);
  assert(source.capacity() == 130);
  assert(source.size() == 130);

  source.expand(200);
  assert(source.capacity() == 200);
  assert(source.size() == 130);
  assert(source.remaining() == 70);

  // source_buffer can be sliced into a slice_buffer
  source.clear();
  source.append("GET / HTTP/1.1");
  auto slice = source.slice(0, 3);
  assert(is_instance_of<warp::slice_buffer>(&slice));
  assert(slice.to_string() == "GET");

  // slice_buffer can further be sliced into another slice_buffer
  auto subslice = slice.slice(1, 2);
  assert(is_instance_of<warp::slice_buffer>(&subslice));
  assert(subslice.to_string() == "E");
}