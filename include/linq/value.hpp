//
//  value.hpp
//  Linq
//
//  Created by Calum Grant on 22/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_value_hpp
#define Linq_value_hpp

namespace linq
{
	template<typename T>
	class value_t : public methods<value_t<T>, ireversible<T>>
	{
	public:
		typedef T value_type;
		value_t(const value_type &v) : value(v) { }

		const value_type & get_value() const { return value; }

		bool move_first() const { return true; }
		bool move_next() const { return false; }
		bool move_prev() const { return false; }
		bool move_last() const { return true; }
	private:
		value_type value;
	};

	template<typename T>
	value_t<T> value(const T&v)
	{
		return value_t<T>(v);
	}

}

#endif
