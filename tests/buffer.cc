#include <algorithm> // std::copy_n
#include <cassert> // assert
#include <cstring> // memcpy
#include <warp/buffer> // warp::source_buffer

template<typename base, typename T>
auto is_instance_of(T const * const ptr) {
  return dynamic_cast<const base*>(ptr) != nullptr;
}

int main() {
  auto source = warp::source_buffer{100};
  assert(source.capacity() == 100);
  assert(source.data() != nullptr);
  assert(source.size() == 0);
  assert(source.to_string() == "");

  // can append on empty buffer
  source.append("hello");
  assert(source.capacity() == 100);
  assert(source.data() != nullptr);
  assert(source.size() == 5);
  assert(source.to_string() == "hello");

  // can append on non-empty buffer
  source.append(" world");
  assert(source.capacity() == 100);
  assert(source.data() != nullptr);
  assert(source.size() == 11);
  // std::cout << source.to_string();
  assert(source.to_string() == "hello world");

  // can clear without reducing capacity
  source.clear();
  assert(source.capacity() == 100);
  assert(source.data() != nullptr);
  assert(source.size() == 0);
  assert(source.to_string() == "");

  // can append on cleared buffer
  source.append("yoyo");
  assert(source.capacity() == 100);
  assert(source.data() != nullptr);
  assert(source.size() == 4);
  assert(source.to_string() == "yoyo");

  // works with std::copy_n
  std::copy_n("cp", 2, source.data());
  assert(source.capacity() == 100);
  assert(source.size() == 4);
  assert(source.to_string() == "cpyo");
  source.resize(2);
  auto cursor = source.cursor();
  source.resize(4);
  std::copy_n("cp", 2, cursor);
  assert(source.to_string() == "cpcp");

  // works with memcpy
  memcpy(source.data(),"mc",2);
  assert(source.capacity() == 100);
  assert(source.size() == 4);
  assert(source.to_string() == "mccp");
  source.resize(2);
  cursor = source.cursor();
  source.resize(4);
  memcpy(cursor, "mc", 2);
  assert(source.to_string() == "mcmc");

  // cursor == data on cleared buffer
  source.clear();
  assert(source.cursor() == source.data());

  // source_buffer can be sliced into a const_buffer
  source.clear();
  source.append("GET / HTTP/1.1");
  auto slice = source.slice(0, 3);
  assert(is_instance_of<warp::const_buffer>(&slice));
  assert(slice.to_string() == "GET");

  // const_buffer can further be sliced into another const_buffer
  auto subslice = slice.slice(1, 2);
  assert(is_instance_of<warp::const_buffer>(&subslice));
  assert(subslice.to_string() == "E");

  // dynamically expands if buffer is too small without losing contents
  auto expander = warp::source_buffer{2};
  expander.append("hi"); // fits within capacity
  expander.append(" there"); // forces new memory allocation
  assert(expander.to_string() == "hi there");
}