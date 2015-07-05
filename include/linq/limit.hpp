//
//  limit.hpp
//  Linq
//
//  Created by Calum Grant on 22/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_limit_hpp
#define Linq_limit_hpp

namespace linq
{
	// Skips over a number of items 
	template<typename Enumerable>
	class skip_t : public methods<skip_t<Enumerable>, ienumerable<typename Enumerable::value_type>>
	{
	public:
		skip_t(Enumerable s, int n) : source(s), to_skip(n) { }

		typedef typename Enumerable::value_type value_type;

		const value_type & get_value() const { return source.get_value(); }

		bool move_first() const
		{
			if(!source.move_first()) return false;
			for(int i=0; i<to_skip; ++i)
				if(!source.move_next()) return false;
			return true;
		}

		bool move_next() const
		{
			return source.move_next();
		}

	private:
		Enumerable source;
		int to_skip;
	};

	// Limits the number of items
	template<typename Enumerable>
	class limit_t : public methods<limit_t<Enumerable>, ienumerable<typename Enumerable::value_type>>
	{
	public:
		typedef typename Enumerable::value_type value_type;

		limit_t(Enumerable src, int count) : source(src), m_count(count) { }

		bool move_first() const
		{
			remaining=m_count;
			return remaining>0 && source.move_first();
		}

		bool move_next() const
		{
			return --remaining >0 && source.move_next();
		}

		const value_type & get_value() const { return source.get_value(); }

	private:
		Enumerable source;
		const int m_count;
		mutable int remaining;
	};
}


#endif
