# linq
Yet another C++ LINQ.

## Overview
A header-only C++ library to provide a simple yet very useful, efficient and robust LINQ.

Language in-line query (LINQ) was originally developed for C# as a way to elegantly express operations on collections, using a SQL-style notation. This often makes code much more elegant. LINQ doesn't provide any new features which couldn't be implemented by hand in just a few lines more, but instead makes many common tasks involving collections much simpler and nicer.

## A quick example

```
#include <linq.hpp>

int main(int argc, char**argv)
{
   std::cout << "The sum of the numbers is " << 
    from(argv, argc).
      where([] (const char *x) { return isdigit(*x); }).
      select([] (const char *x) { return atoi(x); }).
      sum();
}
```
the equivalent in normal C/C++ is as follows: 
```
int main(int argc, char**argv)
{
  int sum=0;
  for(int arg=0; arg<argc; ++arg)
  {
    if(isdigit(*argv[arg]))
    {
      sum += atoi(argv[arg]);
    }
  }
  std::cout << "The sum of the numbers is " << sum;
}
```

## Main features

(Why you should choose this LINQ over others)

* Header-only, so is easily integrated into a project.
* Cross platform, pure C++.
* Generally does not need any (heap-based) memory allocation.
* Low overhead.
* Models C# interface (no ugly ```>>``` operator overloading).
* Can store LINQ expressions in ```auto```, without heap allocation.
* Provides ```linq::enumerator<T>``` interface for method parameters, avoiding templating the target method or memory allocation.
