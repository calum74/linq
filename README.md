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

# Tutorial

LINQ is a convenience library to make common tasks on collections/containers simpler and more robust. LINQ also provides ```linq::enumerable<T>```, which is a convenient abstraction for passing collections between module boundaries.

The basic form of a LINQ query is

```
	from(X).where(Y).select(Z)
```
X can be any 
	


## Main features and benefits

(Why you should choose this LINQ over others)

* Header-only, so is easily integrated into a project.
* Cross platform, pure C++.
* Generally does not need any (heap-based) memory allocation.
* Low overhead.
* Models C# interface (no ugly ```>>``` operator overloading).
* Can store LINQ expressions in ```auto```, without heap allocation.
* Provides ```linq::enumerator<T>``` interface for method parameters, avoiding templating the target method or memory allocation.


# Implementation notes

## Design

All LINQ classes derive from ```linq::enumerable<T>```, which is an enumerable collection of type ```T```. The motivation for this approach is that it allows methods to have parameters of type ```const enumerable<T>&```, which can be compiled separately, as opposed to being templated in header files. This in turn leads to a nicer application design because your API can be passed an ```enumerable<T>``` as opposed to something more specific like a pointer, array or specific container.

In this example, where ```args``` is an enumerable, the ```where()``` clause further refines the collection before passing the result to the ```parse_options()``` function.

```
	void parse_options(const linq::enumerable<const char*> & args);

	parse_options(args.where([](const char * arg) { return arg[0]=='-'; }));
```

This is efficient because the temporary object created by ```where``` lives on the stack so there is no heap allocation, and the compiler has a good chance to optimize it.

It is also possible to store linq objects in ```auto``` variables, i.e.

```
	auto options = args.where([](const char * arg) { return arg[0]=='-';});
```
	
## Enumerables vs Enumerators

The interface (```linq::enumerable<T>```) is iterable using a ```for``` statement, i.e.

```
	void print_options(const linq::enumerable<const char*> & options)
	{
		for(auto i : options)
			std::cout << "Option: " << i << std::endl;
	}
```

To make this efficient, each ```enumerable<>``` includes an iterator, so an ```enumerable<>``` in C++ is a combination of ```System.Linq.IEnumerable<T>``` and ```System.Linq.IEnumerator<T>``` in C#. This sacrificies a little convenience (for example it's not possible to iterate over the same collection in two nested loops), but this is actually a very rare requirement. The alternative would be to dynamically allocate an iterator/enumerator each iteration, which is less efficient.

This sometimes leads to wierd behaviour, for example ```e==e``` can evaluate to false.


	
## Forwards and backwards iteration

Each ```enumerable<>``` can be iterated forwards and backwards, and ```reverse()``` will attempt to iterate the enumerable backwards. However a few enumerables don't actually support backwards iteration (nor do they support being iterated twice over). In this case, the backwards iteration will appear to be empty, i.e.

```
	x.count() != x.reverse().count();
```

If reverse iteration is really needed, use the ```copy()``` method to create a copy of the data (stored internally in a ```std::vector<>```) which will have the desired behaviour.

## Notation and API

This implementation of LINQ uses a fluent API as opposed to an operator-based API. This was mainly for consistency with the C# LINQ library, and is purely a matter of personal taste.

The slight drawback is that C++ does not support extension methods, so creating new LINQ methods means editing ```linq::enumerable<>``` and ```linq::linq``` classes.

## Safety and memory management

All LINQ data structures are at ```auto``` scope, so be aware of dangling pointers. Generally it's hard to create a dangling pointer because the base type ```linq::enumerator``` is abstract, and the 

As expected, the following code is unsafe:
```
	const enumerator<int> & ages = people.select(&person::age);
```
Some LINQ expressions contain references to data which may go out of scope, such as

```
	linq::from(Container);
	linq::from(Iterator, Iterator);
```


A few methods require memory allocation. They are:

* copy()
* sort()
* order_by()

## Algorithmic efficiency

count() is O(n)



## Efficiency

