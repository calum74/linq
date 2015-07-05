//
//  iterator_pair.hpp
//  Linq
//
//  Created by Calum Grant on 22/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_iterator_pair_hpp
#define Linq_iterator_pair_hpp

namespace linq
{

	template<typename It>
	class iterator_pair :
		public methods<iterator_pair<It>, typename util::enumerable_type<It>::iface>
	{
	public:
		iterator_pair(It from, It to) : from(from), to(to)
		{
		}

		typedef typename std::iterator_traits<It>::value_type value_type;
		typedef const value_type & get_value_type;

		bool move_first() const
		{
			current = from;
			return current!=to;
		}

		bool move_next() const
		{
			return to != ++current;
		}

		bool move_prev() const
		{
			It from2 = from;
			return (--current) != (--from2);
		}

		bool move_last() const
		{
			current = to;
			return move_prev();
		}

		const value_type & get_value() const { return *current; }

	private:
		It from, to;
		mutable It current;
	};


	template<typename It>
	iterator_pair<It> from(std::pair<It,It> p)
	{
		return iterator_pair<It>(p.first, p.second);
	}

	template<typename It>
	iterator_pair<It> from(It a, It b)
	{
		return iterator_pair<It>(a,b);
	}

	template<typename It>
	iterator_pair<It> from(It a, int len)
	{
		return iterator_pair<It>(a,a+len);
	}

	inline iterator_pair<const char*> from(const char * str)
	{
		return iterator_pair<const char*>(str, str+std::strlen(str));
	}
}

#endif
