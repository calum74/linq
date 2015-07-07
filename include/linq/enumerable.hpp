//
//  enumerable.hpp
//  Linq
//
//  Created by Calum Grant on 20/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_enumerable_hpp
#define Linq_enumerable_hpp

namespace linq
{
	// !! Split up this file
	template<typename T>
	class sequence_ptr : public methods<sequence_ptr<T>, sequence<T>>
	{
	public:
		typedef std::shared_ptr<isequence<T>> ptr_type;
		typedef T value_type;

		template<typename Derived, typename Base>
		sequence_ptr(const methods<Derived, Base> &e) : m_ptr(std::make_shared<Derived>(e.get()))
		{
		}

		template<typename Derived, typename Base>
		sequence_ptr(methods<Derived, Base> &&e) : m_ptr(std::make_shared<Derived>(std::move(e)))
		{
		}

		//sequence_ptr(const ptr_type & p) : m_ptr(p) { }
		//sequence_ptr(ptr_type && p) : m_ptr(std::move(p)) { }

		bool move_first() const { return m_ptr->move_first(); }
		bool move_next() const { return m_ptr->move_next(); }
		const value_type & get_value() const { return m_ptr->get_value(); }
	private:
		std::shared_ptr<isequence<T>> m_ptr;
	};


	template<typename T>
	bool operator==(const isequence<T> &e1, const isequence<T> &e2)
	{
		bool v1, v2;
		for(v1=e1.move_first(), v2=e2.move_first(); v1&&v2; v1=e1.move_next(), v2=e2.move_next())
		{
			if(e1.get_value() != e2.get_value()) return false;
		}

		return v1==v2;
	}

	template<typename T>
	std::ostream & operator<<(std::ostream & os, const isequence<T> &e)
	{
		os << '{';
		int pos=0;
		for(bool valid = e.move_first(); valid; valid = e.move_next())
		{
			if(pos>0) os << ',';
			os << ' ';
			++pos;
			os << e.get_value();
		}
		if(pos>0) os << ' ';
		os << '}';
		return os;
	}
}

#endif
