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
		public methods<iterator_pair<It>, sequence<typename std::iterator_traits<It>::value_type>>
	{
	public:
		iterator_pair(It from, It to) : from(from), to(to)
		{
		}

		typedef typename std::iterator_traits<It>::value_type value_type;

		bool move_first() const
		{
			current = from;
			return current!=to;
		}

		bool move_next() const
		{
			return to != ++current;
		}

		const value_type & get_value() const { return *current; }

	private:
		It from, to;
		mutable It current;
	};

	template<typename It>
	class iterator_from :
		public methods<iterator_from<It>, sequence<typename std::iterator_traits<It>::value_type>>
	{
	public:
		iterator_from(It s) : from(s) { }

		typedef typename std::iterator_traits<It>::value_type value_type;

		bool move_first() const
		{
			current = from;
			return true;
		}

		bool move_next() const
		{
			++current;
			return true;
		}

		const value_type & get_value() const { return *current; }

	private:
		It from;
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

	template<typename It>
	iterator_from<It> from_unbounded(It it)
	{
		return iterator_from<It>(it);
	}
}

#endif
