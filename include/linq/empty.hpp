//
//  empty.hpp
//  Linq
//
//  Created by Calum Grant on 22/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_empty_hpp
#define Linq_empty_hpp

namespace linq
{
	template<typename T>
	class empty : public methods<empty<T>, ireversible<T>>
	{
	public:
		typedef T value_type;

		bool move_first() const { return false; }
		bool move_next() const { return false; }
		bool move_prev() const { return false; }
		bool move_last() const { return false; }

		const value_type & get_value() const { return *(const value_type*)nullptr; }
	};
}

#endif
