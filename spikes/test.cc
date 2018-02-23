#include <iostream>

struct point {
  int x = 0;
};

int main(int argc, char const *argv[])
{
  
  point origin = { .x = 1 };

  std::cout << origin.x << std::endl;
}