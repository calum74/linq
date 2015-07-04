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

	/*	An enumerator is a movable pointer/cursor into a sequence.
	*/
	template<typename T>
	class enumerableDeleteme
	{
	public:
		typedef T value_type;
		typedef int size_type;
		typedef const value_type & const_reference;

		// Operations to move the current position in the sequence:
		// Note the slightly odd use of "const" - needed to allow
		// enumerators to be passed into functions/methods.
		// Return true if the reference is valid,
		// false if the reference is beyond the sequence or the sequence is empty.
		// Also false if the operation is not supported (e.g. if the underlying
		// data structure only supports a forwards iterator.
		virtual bool move_first() const=0;
		virtual bool move_next() const=0;

		// Gets the current value.
		// Result is undefined if !valid().
		virtual const_reference get_value() const=0;

		class iterator
		{
		public:
			typedef value_type value_type;
			typedef std::forward_iterator_tag iterator_category;
			typedef const value_type * pointer;
			typedef int difference_type;
			typedef const value_type & reference;


			iterator(const enumerable<T> *e) : e(e->move_first() ? e : nullptr)
			{
			}

			iterator() : e(nullptr) { }

			bool operator==(const iterator &a) const { return e==a.e; }
			bool operator!=(const iterator &a) const { return e!=a.e; }

			const_reference operator*() const { return e->get_value(); }
			iterator operator++() { if(!e->move_next()) e=nullptr; return *this; }
			iterator operator--() { if(!e->move_prev()) e=nullptr; return *this; }
		private:
			const enumerable<T> * e;
		};

		typedef iterator const_iterator;

		iterator begin() const { return iterator(this); }
		iterator end() const { return iterator(); }
		iterator cbegin() const { return iterator(this); }
		iterator cend() const { return iterator(); }


		size_type count() const
		{
			size_type c=0;
			for(bool valid=move_first(); valid; valid=move_next())
				++c;
			return c;
		}

		// std::shared_ptr<enumerable<T>> make_shared()


		template<typename Predicate>
		size_type count(Predicate p) const
		{
			size_type c=0;
			for(bool valid=move_first(); valid; valid=move_next())
			{
				if(p(get_value())) ++c;
			}
			return c;
		}


		bool any() const
		{
			return move_first();
		}

		template<typename Predicate>
		bool any(Predicate p) const
		{
			for(bool valid=move_first(); valid; valid=move_next())
			{
				if(p(get_value())) return true;
			}
			return false;
		}

		template<typename Fn>
		value_type aggregate(Fn f = std::plus<value_type>(), typename util::nonconst<value_type>::type total = value_type()) const
		{
			for(bool valid=move_first(); valid; valid=move_next())
				total = f(total, get_value());
			return total;
		}

		value_type sum() const
		{
			return aggregate(std::plus<value_type>());
		}

		value_type product() const
		{
			return aggregate(std::multiplies<value_type>());
		}


		typedef enumerable_ref<value_type> enumerator_ref;

		template<typename P>
		::linq::linq<::linq::where<enumerator_ref,P>> where(P predicate) const
		{
			return ::linq::where<enumerator_ref,P>(*this, predicate);
		}

		template<typename F>
		::linq::linq<::linq::select<enumerator_ref, F>> select(F fn) const
		{
			return ::linq::select<enumerator_ref,F>(enumerator_ref(*this), fn);
		}

		template<typename Struct, typename Field>
		::linq::linq<::linq::select<enumerator_ref, select_field<Struct, Field>>>
		select(Field Struct::*field) const
		{
			return ::linq::select<enumerator_ref, select_field<Struct,Field>>(*this, field);
		}

		::linq::linq<::linq::concat<enumerator_ref, enumerator_ref>>
		concat(const enumerable<value_type> &r) const
		{
			return ::linq::concat<enumerator_ref,enumerator_ref>(*this, r);
		}


		// !! Allocator

		template<typename Selector>
		::linq::linq<::linq::sort<value_type>> order_by(Selector s) const
		{
			auto fn = [=](const value_type &a, const value_type &b) { return s(a)<s(b); };
			return ::linq::sort<value_type>(*this, fn);
		}

		template<typename Struct, typename F>
		::linq::linq<::linq::sort<value_type>> order_by(F Struct::*field) const
		{
			auto fn = [=](const value_type &a, const value_type &b) { return a.*field<b.*field; };
			return ::linq::sort<value_type>(*this, fn);
		}

		::linq::linq<::linq::skip<enumerator_ref>> skip(int n) const
		{
			return ::linq::skip<enumerator_ref>(*this,n);
		}

		::linq::linq<::linq::limit<enumerator_ref>> first(int n) const
		{
			return ::linq::limit<enumerator_ref>(*this,n);
		}

		::linq::linq<::linq::skip<enumerator_ref>> last(size_type n) const
		{
			return ::linq::skip<enumerator_ref>(*this,count()-n);
		}

		template<typename Predicate>
		linq<::linq::until<enumerator_ref,Predicate>> until(Predicate p) const
		{
			return ::linq::until<enumerator_ref,Predicate>(*this, p);
		}

		linq<::linq::repeat<enumerator_ref>> repeat() const
		{
			return ::linq::repeat<enumerator_ref>(*this);
		}

		linq<::linq::repeat_n<enumerator_ref>> repeat(int n) const
		{
			return ::linq::repeat_n<enumerator_ref>(*this, n);
		}

		// Seems really cruddy to repeat all of these functions here ...

		template<typename Fn>
		linq<::linq::select_many<enumerator_ref,Fn>> select_many(Fn fn) const
		{
			return ::linq::select_many<enumerator_ref,Fn>(*this, fn);
		}
	};

	template<typename T>
	bool operator==(const enumerable<T> &e1, const enumerable<T> &e2)
	{
		bool v1, v2;
		for(v1=e1.move_first(), v2=e2.move_first(); v1&&v2; v1=e1.move_next(), v2=e2.move_next())
		{
			if(e1.get_value() != e2.get_value()) return false;
		}

		return v1==v2;
	}

}

#endif
