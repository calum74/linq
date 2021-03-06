//
//  repeat.hpp
//  Linq
//
//  Created by Calum Grant on 24/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_repeat_hpp
#define Linq_repeat_hpp

namespace linq
{
	template<typename Enumerable>
	class repeat_t : public methods<repeat_t<Enumerable>, sequence<typename Enumerable::value_type>>
	{
	public:
		repeat_t(Enumerable e) : source(e) { }

		typedef typename Enumerable::value_type value_type;

		bool move_first() const { return source.move_first(); }
		bool move_next() const { return source.move_next() || source.move_first(); }

		const value_type & get_value() const { return source.get_value(); }

	private:
		Enumerable source;
	};

	template<typename Enumerable>
	class repeat_n_t : public methods<repeat_n_t<Enumerable>, sequence<typename Enumerable::value_type>>
	{
	public:
		typedef typename Enumerable::value_type value_type;

		repeat_n_t(Enumerable e, int c) : source(e), count(c) { }

		bool move_first() const
		{ iteration=0; return iteration<count && source.move_first(); }

		bool move_next() const
		{
			return source.move_next() || (++iteration<count && source.move_first());
		}

		const value_type & get_value() const { return source.get_value(); }

	private:
		Enumerable source;
		mutable int iteration;
		const int count;
	};
}

#endif
