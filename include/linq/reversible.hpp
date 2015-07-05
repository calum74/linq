//
//  reversible.hpp
//  Linq
//
//  Created by Calum Grant on 23/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_reversible_hpp
#define Linq_reversible_hpp

namespace linq
{

	template<typename T>
	class ireversible : public ienumerable<T>
	{
	public:
		virtual bool move_prev() const=0;
		virtual bool move_last() const=0;

		typedef ireversible<T> interface_type;
		typedef std::bidirectional_iterator_tag iterator_category;
	};

	template<typename T>
	class reversible : public methods<reversible<T>, ireversible<T>>
	{
	public:
		typedef T value_type;
		reversible(const ireversible<T> & ref) : m_ref(ref) { }

		const value_type & get_value() const { return m_ref.get_value(); }
		bool move_first() const { return m_ref.move_first(); }
		bool move_next() const { return m_ref.move_next(); }
		bool move_prev() const { return m_ref.move_prev(); }
		bool move_last() const { return m_ref.move_last(); }

	private:
		const ireversible<T> & m_ref;
	};


}

#endif
