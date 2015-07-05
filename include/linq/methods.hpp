//
//  methods.hpp
//  Linq
//
//  Created by Calum Grant on 02/07/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_methods_hpp
#define Linq_methods_hpp

namespace linq
{
	// !! Think about this
	template<typename Derived, typename Base, typename Generic>
	struct user_hooks
	{
	};


	template<typename Derived, typename Base>
	class methods : public Base // , public user_hooks<Derived, Base, void>
	{
	public:
		typedef int size_type;
		typedef typename Base::value_type value_type;
		typedef const value_type & const_reference;

		const Derived & get() const { return *static_cast<const Derived*>(this); }

		template<typename Fn>
		select_t<Derived,Fn> select(Fn fn) const
		{
			return select_t<Derived,Fn>(get(),fn);
		}


		class iterator
		{
		public:
			typedef value_type value_type;
			typedef typename Base::iterator_category iterator_category;
			typedef const value_type * pointer;
			typedef int difference_type;
			typedef const value_type & reference;

			iterator(const methods *e) : e(e->move_first() ? e : nullptr)
			{
			}

			iterator() : e(nullptr) { }

			bool operator==(const iterator &a) const { return e==a.e; }
			bool operator!=(const iterator &a) const { return e!=a.e; }

			reference operator*() const { return e->get_value(); }
			iterator operator++() { if(!e->move_next()) e=nullptr; return *this; }
			iterator operator--() { if(!e->move_prev()) e=nullptr; return *this; }
		private:
			const methods * e;
		};


		class reverse_iterator
		{
		public:
			typedef value_type value_type;
			typedef typename Base::iterator_category iterator_category;
			typedef const value_type * pointer;
			typedef int difference_type;
			typedef const value_type & reference;

			reverse_iterator(const methods *e) : e(e->move_last() ? e : nullptr)
			{
			}

			reverse_iterator() : e(nullptr) { }

			bool operator==(const reverse_iterator &a) const { return e==a.e; }
			bool operator!=(const reverse_iterator &a) const { return e!=a.e; }

			reference operator*() const { return e->get_value(); }
			reverse_iterator operator++() { if(!e->move_prev()) e=nullptr; return *this; }
			reverse_iterator operator--() { if(!e->move_next()) e=nullptr; return *this; }
		private:
			const methods * e;
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


		template<typename Fn, typename T>
		value_type aggregate(Fn f, T total) const
		{
			for(bool valid=this->move_first(); valid; valid=this->move_next())
				total = f(total, this->get_value());
			return total;
		}

		value_type sum() const
		{
			return aggregate(std::plus<value_type>(), value_type());
		}

		value_type product() const
		{
			return aggregate(std::multiplies<value_type>(), value_type());
		}

		size_type count() const
		{
			return aggregate(
				[](int total, const value_type & item) { return total+1; }, 0);
		}

		// std::shared_ptr<enumerable<T>> make_shared()


		template<typename Predicate>
		size_type count(Predicate p) const
		{
			return aggregate(
				[=](size_type total, const value_type & item)
				{
					return p(item) ? total+1 : total;
				}, size_type());
		}

		template<typename Selector>
		sort_t<value_type> order_by(Selector s) const
		{
			auto fn = [=](const value_type &a, const value_type &b) { return s(a)<s(b); };
			return sort_t<value_type>(*this, fn);
		}

		template<typename Struct, typename F>
		sort_t<value_type> order_by(F Struct::*field) const
		{
			auto fn = [=](const value_type &a, const value_type &b) { return a.*field<b.*field; };
			return sort_t<value_type>(*this, fn);
		}

		template<typename Struct, typename Field>
		select_t<Derived, select_field<Struct, Field>>
		select(Field Struct::*field) const
		{
			return select_t<Derived, select_field<Struct,Field>>(get(), field);
		}

		template<typename Cmp>
		sort_t<value_type> sort(Cmp cmp) const
		{
			return sort_t<value_type>(get(), cmp);
		}

		sort_t<value_type> sort() const
		{
			return sort_t<value_type>(get(), std::less<value_type>());
		}

		value_type first_or_default(value_type def = value_type()) const
		{
			return this->move_first() ? this->get_value() : def;
		}

		reverse_t<Derived> reverse() const
		{
			return reverse_t<Derived>(get());
		}

		value_type last_or_default(value_type def = value_type()) const
		{
			return this->move_last() ? this->get_value() : def;
		}

		skip_t<Derived> skip(int n) const
		{
			return skip_t<Derived>(get(),n);
		}

		limit_t<Derived> first(int n) const
		{
			return limit_t<Derived>(get(),n);
		}

		skip_t<Derived> last(size_type n) const
		{
			return skip_t<Derived>(get(),count()-n);
		}

		template<typename Container>
		void copy_to(Container &c) const
		{
			for(bool valid=this->move_first(); valid; valid=this->move_next())
				c.push_back(this->get_value());
		}

		// !! Allocator
		container_store<std::vector<value_type>> copy() const
		{
			return container_store<std::vector<value_type>>(*this);
		}

		bool any() const
		{
			return this->move_first();
		}

		template<typename Predicate>
		bool any(Predicate p) const
		{
			for(bool valid=this->move_first(); valid; valid=this->move_next())
			{
				if(p(this->get_value())) return true;
			}
			return false;
		}

		template<typename P>
		where_t<Derived, P> where(P pred) const
		{
			return where_t<Derived,P>(get(),pred);
		}

		template<typename E2>
		concat_t<Derived, E2> concat(const E2 & e2) const
		{
			return concat_t<Derived, E2>(get(), e2);
		}

		template<typename Predicate>
		until_t<Derived,Predicate> until(Predicate p) const
		{
			return until_t<Derived,Predicate>(get(), p);
		}

		template<typename Fn>
		select_many_t<Derived,Fn> select_many(Fn fn) const
		{
			return select_many_t<Derived,Fn>(get(), fn);
		}

		repeat_t<Derived> repeat() const
		{
			return repeat_t<Derived>(get());
		}

		repeat_n_t<Derived> repeat(int n) const
		{
			return repeat_n_t<Derived>(get(), n);
		}

		select_t<Derived, keys_t<Derived>> keys() const
		{
			return select_t<Derived, keys_t<Derived>> (get(), keys_t<Derived>());
		}

		select_t<Derived, values_t<Derived>> values() const
		{
			return select_t<Derived, values_t<Derived>> (get(), values_t<Derived>());
		}


	};
}

#endif
