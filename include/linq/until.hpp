//
//  until.hpp
//  Linq
//
//  Created by Calum Grant on 23/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_until_hpp
#define Linq_until_hpp

namespace linq
{
	template<typename Enumerable, typename Predicate>
	class until_t : public methods<until_t<Enumerable,Predicate>, sequence<typename Enumerable::value_type>>
	{
	public:
		typedef typename Enumerable::value_type value_type;

		until_t(Enumerable e, Predicate p) : source(e), predicate(p) { }

		bool move_first() const
		{
			return source.move_first() && !predicate(source.get_value());
		}

		bool move_next() const
		{
			return source.move_next() && !predicate(source.get_value());
		}

		const value_type & get_value() const { return source.get_value(); }

	private:
		Enumerable source;
		Predicate predicate;
	};
}

#endif
