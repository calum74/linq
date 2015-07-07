//
//  container_store.hpp
//  Linq
//
//  Created by Calum Grant on 22/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_container_store_hpp
#define Linq_container_store_hpp

namespace linq
{
	// Takes a copy of a container

	template<typename Container>
	class container_store : public methods<
		container_store<Container>,
		sequence<typename Container::value_type>>
	{
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::const_iterator container_iterator;
		typedef typename Container::allocator_type allocator_type;

		template<typename It>
		container_store(It from, It to, allocator_type alloc = allocator_type()) :
			container(alloc)
		{
			for(It a=from; a!=to; ++a)
				container.push_back(*a);
		}

		container_store(const sequence<value_type> &s, allocator_type alloc = allocator_type()) :
			container(alloc)
		{
			for(const auto &i : s)
				container.push_back(i);
		}

		const value_type &get_value() const { return *current; }

		bool move_first() const
		{
			current = container.begin();
			return current != container.end();
		}

		bool move_next() const
		{
			return (++current) != container.end();
		}

		// !! Efficient reverse ??

	protected:
		Container container;
		mutable typename Container::const_iterator current;
	};
}

#endif
