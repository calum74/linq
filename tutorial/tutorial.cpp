
// Header file for the linq library
#include "linq.hpp"

#include <map>

struct Person
{
	std::string name;
	int age;
};

bool is_even(int x)
{
	return x%2==0;
}

void tutorial()
{
	// Create an enumerable sequence from any container
	std::vector<int> vec;
	linq::from(vec);

	// Create an enumerable range of numbers
	linq::range(1,10);

	// Create an enumerable range 2,4,6,8,10
	linq::range(2,10,2);

	// Create an enumerable sequence from an initalizer list
	linq::from( { 1, 2, 3 } );

	// Create an enumerable sequence from a pair_of_iterators
	std::map<std::string, int> ages;
	linq::from(ages.begin(), ages.upper_bound("Fred"));
	linq::from(ages.equal_range("Fred"));

	// Create an enumerable range from a fixed-size array
	int array[3];
	linq::from(array);

	// Create an enumerable sequence from a pointer
	linq::from(array, 2);

	// Create an enumerable range from a string
	linq::from("Hello, world!");

	// Create an empty sequence
	linq::empty<int>();

	// Create a single-valued sequence
	linq::value(1);

	// Print a list of the form "{ 1, 2, 3 }"
	std::cout << linq::from(array);

	std::vector<Person> people;

	auto centegenarians =
		linq::from(people).
		where([](const Person &p) { return p.age>=100; });

	// Pass a sequence into another module
	void congratulate(linq::v2::enumerable<Person> people);
	congratulate(centegenarians);

	// Enumerate a sequence
	for(const auto & p : centegenarians)
		std::cout << "Congratulations, " << p.name;

	// Filter a sequence using a predicate
	linq::from({1,2,3}).where([](int i) { return i%2==0; });

	// Store a sequence in a local variable
	auto evens = linq::range(1,100).where(is_even);

	// Store an enumerable in a
	std::vector<int> x(2);

	// Select

	// Sum the numbers or strings in a sequence

	int sum_of_ages = linq::from(people).select(&Person::age).sum();

	// Sorting

	// select_many

	// Multi-table joins

	// Inner join

	// linq::from({"Fred","Mary"}).join(linq::from({6,9})); // .select(



}

void congratulate(linq::v2::enumerable<Person> people)
{
	// Enumerate a sequence
	for(const auto & p : people)
		std::cout << "Congratulations, " << p.name;
}