# Chapter 11 - Function Overloading and Function Templates

## 11.1 - Introduction to function overloading

**Function overloading**

```cpp
int add(int x, int y) // integer version
{
    return x + y;
}

double add(double x, double y) // floating point version
{
    return x + y;
}
```

> [!NOTE]
> Functions can be overloaded so long as each overloaded function can be
> differentiated by the compiler. If an overloaded function can not be
> differentiated, a compile error will result.

**Overload resolution**

```cpp
int add(int x, int y)
{
    return x + y;
}

double add(double x, double y)
{
    return x + y;
}

int main()
{
    std::cout << add(1, 2); // calls add(int, int)
    std::cout << add(1.2, 3.4); // calls add(double, double)

    return 0;
}
```

To make it compile,

1. Each overloaded function has to be differentiated from the others.
1. Each call to an overloaded function has to resolve to an overloaded
   function.

> [!TIP]
> Use function overloading to make your program simpler.

> [!NOTE]
> Function overloading slows your compile time by a trivial amount, and has no
> runtime impact.


## 11.2 - Function overload differentiation

Function overloads are _differentiated_ based on

- Number of parameters
    ```cpp
    int add(int x, int y)
    {
        return x + y;
    }

    int add(int x, int y, int z)
    {
        return x + y + z;
    }
    ```
- Types of parameters
    ```cpp
    int add(int x, int y); // integer version
    double add(double x, double y); // floating point version
    double add(int x, double y); // mixed version
    double add(double x, int y); // mixed version
    ```
  - Excludes `typedef`s, type aliases, and `const` qualifier on value
    parameters. Includes ellipses.

    _type aliases_ (or `typedefs`) are not distinct types

    ```cpp
    typedef int Height; // typedef
    using Age = int; // type alias

    void print(int value);
    void print(Age value); // not differentiated from print(int)
    void print(Height value); // not differentiated from print(int)
    ```

    For parameters _passed by value_, the `const` qualifier is also not
    considered.

    ```cpp
    void print(int);
    void print(const int); // not differentiated from print(int)
    ```

    _Ellipsis_ parameters are considered to be a unique type of parameters

    ```cpp
    void foo(int x, int y);
    void foo(int x, ...); // differentiated from foo(int, int)
    // foo(4, 5) will match to foo(int, int), not foo(int, ...)

    // Note however
    void bar(int x)
    void bar(int x, ...)
    bar(3); // ambiguous
    ```

Note that a function's **return type** is _not_ used to differentiate
overloaded functions.

```cpp
int getRandomValue();
double getRandomValue();

// Which of the two overloaded functions would you call?
getRandomValue();

...

// The best way to address this is to give the functions different names:
int getRandomInt();
double getRandomDouble();
```

> [!NOTE]
> This was an intentional choice, as it ensures the behavior of a function call
> can be determined independently from the rest of the expression, making
> understanding complex expressions much simpler. Put another way, we can
> always determine which version of a function will be called based solely on
> the arguments in the function call. If return values were used for
> differentiation, then we wouldn't have an easy syntactic way to tell which
> overload of a function was being called -- we’d also have to understand how
> the return value was being used, which requires a lot more analysis.

Functions can also be overloaded based on function level-qualifiers:

- `const` or `volatile` (not `constexpr`)
- Ref-qualifiers

As an example, a `const` member function can be differentiated from an
otherwise identical non-`const` member function (even if they share the same
set of parameters).

```cpp
class V {
    int getValue() const;
    int getValue();
};
```

**Type signature** (or **signature**): parts of function header that is used
for differentiation. It includes

- function name
- number of parameters
- parameter type
- function-level qualifiers
- It notably does not include the return type.


> [!NOTE]
> When the compiler compiles a function, it performs **name mangling**,
> which means the compiled name of the function is altered ("mangled") based on
> various criteria, such as the number and type of parameters, so that the
> linker has unique names to work with.
>
> For example, a function with prototype int `fcn()` might compile to mangled
> name `__fcn_v`, whereas int `fcn(int)` might compile to mangled name
> `__fcn_i`. So while in the source code, the two overloaded functions share
> the name `fcn()`, in compiled code, the mangled names are unique (`__fcn_v`
> vs `__fcn_i`).
>
> There is no standardization on how names should be mangled, so different
> compilers will produce different mangled names.
>
> The mangled names in the compiled code must be unique to prevent linker
> errors (otherwise the linker will think we've defined the same function more
> than once, which violates the ODR).

> [!TIP]
> Names can be "demangled" using the following command:
>
> ```console
> $ strings myExecutable | c++filt
> ```
>
> or
>
> ```console
> $ gcc -c filename.cpp -o filename.o
> $ objdump -D filename.o > instructions.txt  # contains mangled names
> $ c++filt __Z3addid  # add(int, double)
> ```

> [!NOTE]
> [**Foriend Function Interface** (FFI)](https://en.wikipedia.org/wiki/Foreign_function_interface):
>
> To expose C++ functions to C (which doesn't support function overloading) and
> other languages, (`extern "C"`) syntax is used which basically tells the
> compiler to NOT consider number and type of parameters in function name
> mangling. That means, you can't overload a function declared as `extern "C"`.
>
> TODO: example

---

My thoughts:

Refs

```cpp
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
}
```

Namespaces

```cpp
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
```

## References:

1. [learncpp.com](learncpp.com)
