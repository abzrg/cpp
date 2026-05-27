
#include <iostream>

void add(int &x) { std::cout << "ref\n"; }
void add(int x) { std::cout << "value\n"; }
// void add(int i, ...) { /* ellipsis */ }



void swap(int& a, int& b)
{
    std::swap(a, b);
    std::cout << "swap() called.\n"; // doesn't execute
}

int main()
{
    using std::swap; // From now on, swap should match std::swap
    int x{ 10 };
    int y{ 20 };
    swap(x, y); // std::swap
}

int main2() {
  int a = 11;

  add(1); //> value
  // add(a); //> ambiguous
  // add((int&)a); // ambiguous
  add((int&&)a); // value
  static_cast<void (*)(int &)>(&add)(a); // ref (DON'T)

  return 0;
}