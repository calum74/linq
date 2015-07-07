//
//  range.hpp
//  Linq
//
//  Created by Calum Grant on 17/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_range_hpp
#define Linq_range_hpp
namespace linq
{
	template<typename T>
	class range_t : public methods<range_t<T>, sequence<T>>
	{
	private:
		T from, stride, to;
		mutable T current;

	public:
		typedef T value_type;
		// typedef reversible<value_type> interface_type;

		range_t(T from, T to, T stride) : from(from), to(to), stride(stride)
		{
		}

		bool move_first() const
		{
			current = from;
			return true;
		}

		bool move_next() const
		{
			if(current==to) return false;
			else
			{
				current+=stride;
				return true;
			}
		}

		bool move_prev() const
		{
			if(current==from)
				return false;
			else
			{
				current -= stride;
				return true;
			}
		}

		bool move_last() const
		{
			current = to;
			return true;
		}

		const T &get_value() const { return current; }
	};


	template<typename T>
	range_t<T> range(T from, T to, T stride=1)
	{
		return range_t<T>(from, to, stride);
	}
}

#endif
