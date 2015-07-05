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
	template<typename T>
	class ienumerable
	{
	public:
		typedef T value_type;
		virtual const value_type & get_value() const=0;
		virtual bool move_first() const=0;
		virtual bool move_next() const=0;

		typedef ienumerable<T> interface_type;
		typedef std::forward_iterator_tag iterator_category;
	};


	template<typename T>
	class enumerable : public methods<enumerable<T>, ienumerable<T>>
	{
	public:
		typedef T value_type;
		enumerable(const ienumerable<T> & ref) : m_ref(ref) { }

		const value_type & get_value() const { return m_ref.get_value(); }
		bool move_first() const { return m_ref.move_first(); }
		bool move_next() const { return m_ref.move_next(); }

	private:
		const ienumerable<T> & m_ref;		// ?? Maybe make it assignable ?? 
	};


	template<typename Base>
	class enumerable_ptr : public methods<enumerable_ptr<Base>, Base>
	{
	public:
		typedef std::shared_ptr<Base> ptr_type;
		typedef typename Base::value_type value_type;
		enumerable_ptr(const ptr_type & p) : m_ptr(p) { }

		bool move_first() const { return m_ptr->move_first(); }
		bool move_next() const { return m_ptr->move_next(); }
		bool move_prev() const { return m_ptr->move_prev(); }
		bool move_last() const { return m_ptr->move_last(); }
		const value_type & get_value() const { return m_ptr->get_value(); }
	private:
		std::shared_ptr<Base> m_ptr;
	};

	template<typename T>
	bool operator==(const ienumerable<T> &e1, const ienumerable<T> &e2)
	{
		bool v1, v2;
		for(v1=e1.move_first(), v2=e2.move_first(); v1&&v2; v1=e1.move_next(), v2=e2.move_next())
		{
			if(e1.get_value() != e2.get_value()) return false;
		}

		return v1==v2;
	}

	template<typename T>
	std::ostream & operator<<(std::ostream & os, const ienumerable<T> &e)
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
