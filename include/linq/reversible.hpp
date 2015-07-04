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
	class reversible_ref : reversible<T>
	{
	public:
		reversible_ref(const reversible<T> &r) : m_ref(r) { }
		typedef reversible_tag enumerable_type;

		typedef T value_type;
		typedef const value_type & const_reference;

		bool move_first() const { return m_ref.move_first(); }
		bool move_next() const { return m_ref.move_next(); }
		bool move_prev() const { return m_ref.move_prev(); }
		bool move_last() const { return m_ref.move_last(); }
		const_reference get_value() const { return m_ref.get_value(); }

	private:
		const reversible<T> &m_ref;
	};


	template<typename T>
	class reversible : public enumerable<T> // , public v2::decorate<reversible<T>, v2::ireversible<T>>
	{
	public:

		typedef T value_type;
		virtual bool move_prev() const=0;
		virtual bool move_last() const=0;

		class iterator
		{
		public:
			typedef value_type value_type;
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef const value_type * pointer;
			typedef int difference_type;
			typedef const value_type & reference;


			iterator(const reversible<T> *e) : e(e->move_first() ? e : nullptr)
			{
			}

			iterator() : e(nullptr) { }

			bool operator==(const iterator &a) const { return e==a.e; }
			bool operator!=(const iterator &a) const { return e!=a.e; }

			reference operator*() const { return e->get_value(); }
			iterator operator++() { if(!e->move_next()) e=nullptr; return *this; }
			iterator operator--() { if(!e->move_prev()) e=nullptr; return *this; }
		private:
			const reversible<T> * e;
		};


		class reverse_iterator
		{
		public:
			typedef value_type value_type;
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef const value_type * pointer;
			typedef int difference_type;
			typedef const value_type & reference;

			reverse_iterator(const reversible<T> *e) : e(e->move_last() ? e : nullptr)
			{
			}

			reverse_iterator() : e(nullptr) { }

			bool operator==(const reverse_iterator &a) const { return e==a.e; }
			bool operator!=(const reverse_iterator &a) const { return e!=a.e; }

			reference operator*() const { return e->get_value(); }
			reverse_iterator operator++() { if(!e->move_prev()) e=nullptr; return *this; }
			reverse_iterator operator--() { if(!e->move_next()) e=nullptr; return *this; }
		private:
			const reversible<T> * e;
		};

		typedef iterator const_iterator;
		typedef reverse_iterator const_reverse_iterator;

		iterator begin() const { return iterator(this); }
		iterator end() const { return iterator(); }
		reverse_iterator rbegin() const { return reverse_iterator(this); }
		reverse_iterator rend() const { return reverse_iterator(); }

		iterator cbegin() const { return iterator(this); }
		iterator cend() const { return iterator(); }
		reverse_iterator crbegin() const { return reverse_iterator(this); }
		reverse_iterator crend() const { return reverse_iterator(); }


		typedef reversible_ref<T> reversible_ref;

		::linq::linq<::linq::reverse<reversible_ref>> reverse() const
		{
			return ::linq::reverse<reversible_ref>(*this);
		}

		value_type last_or_default(value_type def = value_type()) const
		{
			return move_last() ? this->get_value() : def;
		}
	};
}

#endif
