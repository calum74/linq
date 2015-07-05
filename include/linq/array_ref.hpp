//
//  array_ref.hpp
//  Linq
//
//  Created by Calum Grant on 22/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_array_ref_hpp
#define Linq_array_ref_hpp

namespace linq
{	template<typename T, size_t N>
	class array_ref : public methods<array_ref<T,N>, ireversible<T>>
	{
	public:
		array_ref(T*data) : data(data) { }

		typedef T value_type;

		const value_type & get_value() const { return *current; }

		bool move_first() const
		{
			current = data;
			return N>0;
		}

		bool move_last() const
		{
			current = data+N-1;
			return N>0;
		}

		bool move_next() const
		{
			++current;
			return current <data+N;
		}

		bool move_prev() const
		{
			--current;
			return current>=data;
		}
	private:
		T * data;
		mutable T * current;
	};

	template<typename T, std::size_t N>
	array_ref<T,N> from(T (&x)[N])
	{
		return array_ref<T,N>(x);
	}

	// Character arrays are assumed to be null-terminated.
	// this is honestly a bit of a gotcha because it's inconsistent with
	// other array types.
	template<std::size_t N>
	array_ref<const char,N-1> from(const char (&x)[N])
	{
		return array_ref<const char,N-1>(x);
	}

	template<std::size_t N>
	array_ref<const wchar_t,N-1> from(const wchar_t (&x)[N])
	{
		return array_ref<const char,N-1>(x);
	}

}

#endif
