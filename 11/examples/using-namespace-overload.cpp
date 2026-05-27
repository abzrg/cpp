
#include <algorithm>
#include <iostream>

void swap(int &a, int &b) {
  std::swap(a, b);
  std::cout << "swap() called.\n"; // doesn't execute
}

int main() {
  using std::swap; // From now on, swap should match std::swap
  int x{10};
  int y{20};
  swap(x, y); // std::swap
}