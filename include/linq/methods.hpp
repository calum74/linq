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
	template<typename Derived, typename Generic>
	struct user_hooks
	{
	};

	template<typename Derived, typename Base, typename Stored>
	class methods : public Base, public user_hooks<Derived, void>
	{
	public:
		typedef int size_type;
		typedef typename Base::value_type value_type;
		typedef const value_type & const_reference;
		typedef Stored stored_type;

		const Derived & get() const { return *static_cast<const Derived*>(this); }

		template<typename Fn>
		select_t<stored_type,Fn> select(Fn fn) const
		{
			return select_t<stored_type,Fn>(get(),fn);
		}


		class iterator
		{
		public:
			typedef value_type value_type;
			typedef typename std::forward_iterator_tag iterator_category;
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


		typedef iterator const_iterator;

		iterator begin() const { return iterator(this); }
		iterator end() const { return iterator(); }

		iterator cbegin() const { return iterator(this); }
		iterator cend() const { return iterator(); }


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
		select_t<stored_type, select_field<Struct, Field>>
		select(Field Struct::*field) const
		{
			return select_t<stored_type, select_field<Struct,Field>>(get(), field);
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

		// !! Alloc version
		reverse_t<value_type> reverse() const
		{
			return reverse_t<value_type>(get());
		}

		value_type last_or_default(value_type def = value_type()) const
		{
			for(bool valid = this->move_first(); valid; valid = this->move_next())
			{
				def = this->get_value();
			}
			return def;
		}

		skip_t<stored_type> skip(int n) const
		{
			return skip_t<stored_type>(get(),n);
		}

		limit_t<stored_type> first(int n) const
		{
			return limit_t<stored_type>(get(),n);
		}

		skip_t<stored_type> last(size_type n) const
		{
			return skip_t<stored_type>(get(),count()-n);
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
			return container_store<std::vector<value_type>>(get());
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
		where_t<stored_type, P> where(P pred) const
		{
			return where_t<stored_type,P>(get(),pred);
		}

		template<typename E2>
		concat_t<stored_type, typename E2::stored_type> concat(const E2 & e2) const
		{
			return concat_t<stored_type, typename E2::stored_type>(get(), e2);
		}

		template<typename Predicate>
		until_t<stored_type,Predicate> until(Predicate p) const
		{
			return until_t<stored_type,Predicate>(get(), p);
		}

		template<typename Fn>
		select_many_t<stored_type,Fn> select_many(Fn fn) const
		{
			return select_many_t<stored_type,Fn>(get(), fn);
		}

		repeat_t<stored_type> repeat() const
		{
			return repeat_t<stored_type>(get());
		}

		repeat_n_t<stored_type> repeat(int n) const
		{
			return repeat_n_t<stored_type>(get(), n);
		}

		select_t<stored_type, keys_t<stored_type>> keys() const
		{
			return select_t<stored_type, keys_t<stored_type>> (get(), keys_t<Derived>());
		}

		select_t<stored_type, values_t<stored_type>> values() const
		{
			return select_t<stored_type, values_t<stored_type>> (get(), values_t<Derived>());
		}

		sequence_ptr<value_type> ptr() const
		{
			return sequence_ptr<value_type>(get());
		}
	};
}

#endif
