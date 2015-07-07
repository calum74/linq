//
//  where.hpp
//  Linq
//
//  Created by Calum Grant on 18/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_where_hpp
#define Linq_where_hpp


namespace linq
{
	template<typename Enumerable, typename Predicate>
	class where_t : public
		methods<where_t<Enumerable,Predicate>, sequence<typename Enumerable::value_type> >
	{
		Enumerable source;
		Predicate predicate;
	public:
		where_t(Enumerable s, Predicate p) : source(s), predicate(p) { }
		// where_t(where_t && a) : source(std::move(a.source)), predicate(std::move(a.predicate)) { }

		typedef typename Enumerable::value_type value_type;

		bool move_first() const
		{
			for(bool valid=source.move_first();
				valid ;
				valid=source.move_next())
			{
				if(predicate(source.get_value())) return true;
			}
			return false;
		}

		bool move_last() const
		{
			for(bool valid=source.move_last();
				valid ;
				valid=source.move_prev())
			{
				if(predicate(source.get_value())) return true;
			}
			return false;
		}


		bool move_next() const
		{
			for(bool valid=source.move_next(); valid; valid=source.move_next())
			{
				if(predicate(source.get_value())) return true;
			}
			return false;
		}

		bool move_prev() const
		{
			for(bool valid=source.move_prev(); valid; valid=source.move_prev())
			{
				if(predicate(source.get_value())) return true;
			}
			return false;
		}

		const value_type & get_value() const
		{
			return source.get_value();
		}
	};
}

#endif
