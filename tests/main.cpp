//
//  main.cpp
//  Linq
//
//  Created by Calum Grant on 14/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#include "linq.hpp"

#include <map>
#include <string>
#include <list>

#include <cassert>


template<typename E1, typename E2>
class my_zip
{
	E1 e1;
	E2 e2;
};

// Generic tests on the enumerable { 1,2,3 }
template<typename E>
void test_enumerable2(const E &e)
{
	auto e2 = e.copy();
	assert(e == linq::range(1,3));
	assert(e.count()==3);
	assert(e.sum()==6);

	auto c = e.where([](int x) { return x!=2; });
	assert(c.count()==2);
	assert(c.sum()==4);
	auto d = e.select([](int x) { return x*2; });
	assert( d.sum() == 12 );

	auto concat = e.concat(e);

	assert( concat.count()==6 );

	int x=0;
	e.end();
	for(auto i : e)
		x+=i;
	assert(x==6);

	assert( e.count([](int x) { return x<=2; }) == 2 );

	assert( e.where([] (int x) { return false; }).count()==0 );
	assert( !e.where([] (int x) { return false; }).any() );
	assert( !e.any([](int) { return false; }) );
	assert( e.count([](int) { return false; })==0 );

	assert( e.copy() == e );

	assert(e.first_or_default() == 1);

	assert(e.first(3)==e.copy());
	assert(e.skip(0)==e.copy());

	assert( e.last(1) == linq::from({3}) );
	assert( e.first(1) == linq::from({1}) );

	//if(e.reverse().any())
	//{
	//	assert( e.last_or_default()==3);
	//}

	assert( e.copy() == e.select([](int i) { return i; }));
	assert( e.concat(e) == linq::from({1,2,3,1,2,3}) );

	assert( e.sort() == e );
	assert( e.sort([](int i, int j) { return i>j; }) == linq::from({3,2,1}) );

	assert( linq::from(e) == e.copy() );
	assert( linq::from(e.begin(), e.end()) == e2 );

	assert( e.until([](int x) { return x==3; }) == e.first(2).copy() );

	assert( e.repeat(0) == linq::empty<int>() );
	assert( e.repeat(1) == e.copy() );
	assert( e.repeat().first(5) == linq::range(1,3).concat(linq::range(1,2)) );

	assert( e.select_many( [](int x) { return linq::value(x); } ) == e2 );
	assert( linq::value(1).select_many([&](int x) { return e.copy(); } ) == e2);
}

template<typename E>
void test_reversible2(const E&e)
{
	assert( e.reverse()==linq::from({3,2,1}));

	assert( e.last_or_default() == 3 );

	assert( linq::from(e.rbegin(), e.rend()) == linq::from({3,2,1}));
}

template<typename E>
void test_enumerable(E e)
{
	test_enumerable2(e);
	test_enumerable2(linq::v2::enumerable<int>(e));
	test_enumerable2((const linq::v2::enumerable<int>&)e);
}

template<typename E>
void test_reversible(E e)
{
	test_enumerable(e);
	test_enumerable(e.reverse().reverse());

	test_reversible2(e);
	test_reversible2((const linq::v2::reversible<int>&)e);
}



struct data
{
	int i;
	double d;
	std::string s;
};

std::string enquote(char c)
{
	return {'\'', c, '\''};
}


int main(int argc, const char * argv[])
{
	typedef linq::v2::enumerable<int> intseq;
	typedef linq::v2::reversible<int> reversible_seq;

	// Test the range enumerable
	assert( linq::range(1,3)==linq::range(1,3) );
	assert( linq::range(1,3).count()==3 );
	assert( linq::range(1,3).reverse() == linq::range(3,1,-1) );
	assert( linq::range(1,3).sum() == 6 );

	test_enumerable( linq::range(1,3) );

	std::vector<int> v1 = {1,2,3};
	assert( linq::from(v1) == linq::range(1,3));

	test_enumerable( linq::from(v1) );

	test_enumerable( linq::from(v1.begin(), v1.end()) );

	test_enumerable( linq::range(1,2).concat(linq::range(3,3)).concat(linq::empty<int>()) );

	test_enumerable( linq::value(1).concat(linq::value(2).concat(linq::value(3))) );
	test_enumerable( linq::value(1).concat(linq::value(2)).concat(linq::value(3)) );

	std::map<int,int> m1;
	m1[1]=3;
	m1[2]=2;
	m1[3]=1;
	test_enumerable(linq::from(m1).keys());
	test_enumerable(linq::from(m1).values().reverse());

	test_enumerable(linq::range(-1,3).skip(2));

	std::map<std::string,std::string> m2;
	m2["a"] = "A";
	m2["b"] = "B";

	assert( linq::from(m2).keys().sum() == "ab" );
	assert( linq::from(m2).values().sum() == "AB" );

	// !! Test temporaries.

	int a1[3] = { 1, 2, 3 };
	test_enumerable(linq::from(a1));

	data d1[3] = { {1,2.0,""}, {2,1.0,""}, {3,5.0,""}};
	data d2[3] = { {2,2.0,""}, {3,1.0,""}, {1,5.0,""}};

	// from(d1).select(&data::i);
	test_enumerable(linq::from(d1).select([](const data &d) { return d.i; }));
	test_enumerable(linq::from(d1).select(linq::field(&data::i)));
	test_enumerable(linq::from(d1).select(&data::i));

	test_reversible(linq::from(d2).
		reverse().
		order_by(linq::field(&data::d)).
		order_by(linq::field(&data::i)).
		select(linq::field(&data::i))
		);

	test_reversible(linq::from(d2).
		reverse().
		order_by(&data::d).
		order_by(&data::i).
		select(&data::i)
		);

	test_enumerable(linq::range(1,3).reverse().sort());

	// Make sure all tests are also in test_enumerable

	linq::from(m1.equal_range(2));
	linq::from(m1.lower_bound(1), m1.upper_bound(1));

	test_enumerable(linq::from({1,2,3}));
	test_enumerable(linq::from({3,2,1}).reverse());

	assert( linq::empty<int>().first_or_default(1) == 1 );
	assert( linq::empty<int>().last_or_default(1) == 1 );
	assert( intseq(linq::empty<int>()).first_or_default(1) == 1 );
	assert( reversible_seq(linq::empty<int>()).last_or_default(1) == 1 );

	test_enumerable( linq::range(-1,3).skip(2) );
	test_enumerable( linq::range(1,10).first(3) );
	test_enumerable( intseq(linq::range(1,10)).first(3) );

	test_enumerable( linq::range(-1,3).last(3) );

	test_enumerable( linq::range(1,10).until([](int i) { return i==4; }) );

	int my_array[3] = { 1, 2, 3 };

	test_enumerable(linq::from(my_array,my_array+3));
	test_enumerable2(intseq(linq::from(my_array,my_array+3)));
	test_enumerable(linq::from(my_array,3));

	assert(linq::from("Hello world!") == linq::from((const char*)"Hello world!"));

	assert( linq::range(1,7,3).count()==3 );
	auto many = linq::range(1,7,3).select_many([](int v) { return linq::range(v,v+2); });
	assert( many == linq::range(1,9) );
	test_enumerable(many.first(3));

	auto manymore = linq::range(7,1,-3).select_many([](int v) { return linq::range(v+2,v,-1); });
	assert(many.reverse()==manymore);
	assert(manymore.reverse()==many);
	test_enumerable(manymore.reverse().first(3));
	test_enumerable(manymore.last(3).copy().reverse());


	test_enumerable( linq::from({1,2,3}).repeat().skip(6).first(3) );

	assert( linq::value(1).repeat().first(100).sum() == 100 );
	assert( linq::from({1,2,3}).repeat().first(5) == linq::from({1,2,3,1,2}) );
	assert( linq::from({1,2,3}).repeat().reverse().first(2) == linq::from({3,2}) );

	assert( linq::from({1,2}).repeat(4).sum() == 12 );
	assert( linq::from({1,2}).reverse().repeat(4).sum() == 12 );
	assert( linq::value(1).repeat(100).sum()==100);

	// repeat(int n) - repeat n times
	// repeat() - infinite loop
	// Add more tests to test_enumerable()

	// select_many - more types ...

	// unique()

	// User-defined extension ??

	/*
		my_zip(src1, src2).fu.bar
	*/

	// linq::value(1).select([](int x) { return false; });


	// std::cout << linq::range(1,10).select([](int p) { return linq::from({p,p+1}); });

	//select on a field on an enumerable.

	std::cout << linq::from("Tests passed!").select(enquote) << std::endl;

	// Now for v2

	{
		auto i = linq::range_t<int>(1,3,1);
		linq::v2::enumerable<int> x(i);
	}
}
