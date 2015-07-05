//
//  reverse.hpp
//  Linq
//
//  Created by Calum Grant on 20/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_reverse_hpp
#define Linq_reverse_hpp

namespace linq
{
	template<typename Enumerable>
	class reverse_t : public methods<reverse_t<Enumerable>, typename Enumerable::interface_type>
	{
	public:
		typedef typename Enumerable::value_type value_type;

		reverse_t(Enumerable s) : source(s) { }

		bool move_first() const { return source.move_last(); }
		bool move_next() const { return source.move_prev(); }
		bool move_prev() const { return source.move_next(); }
		bool move_last() const { return source.move_first(); }
		const value_type & get_value() const { return source.get_value(); }
	private:
		Enumerable source;
	};
}

#endif
