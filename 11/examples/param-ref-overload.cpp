#include <iostream>

void add(int &x) { std::cout << "ref\n"; }
void add(int x) { std::cout << "value\n"; }

// All calls will be ambiguous if this signature is used
// void add(int i, ...) { /* ellipsis */ }

int main() {
  int a = 11;

  add(1); //> value
  // add(a); //! ambiguous
  // add((int&)a); //! ambiguous
  add((int&&)a); //> value
  static_cast<void (*)(int &)>(&add)(a); // ref (DON'T)

  return 0;
}