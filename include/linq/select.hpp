//
//  select.hpp
//  Linq
//
//  Created by Calum Grant on 17/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_select_hpp
#define Linq_select_hpp

namespace linq
{
	template<typename S, typename T>
	struct select_field
	{
	public:
		select_field(T S::*f) : field (f) { }
		const T &operator()(const S&s) const { return s.*field; }
	private:
		T S::*field;		// Ideally this would be a template parameter
	};

	template<typename S, typename T>
	select_field<S,T> field(T S::*field)
	{
		return field;
	}

	template<typename Source>
	class keys_t
	{
	public:
		// !! Look up proper types
		// typedef typename Source::value_type arg_type;
		// !! Without the const!!
		typedef typename util::nonconst<typename Source::value_type::first_type>::type result_type;

		const result_type &operator()(const typename Source::value_type &v) const
		{
			return v.first;
		}
	};


	template<typename Source>
	class values_t
	{
	public:
		// !! Look up proper types
		typedef typename Source::value_type arg_type;
		// !! Without the const!!
		typedef typename util::nonconst<typename Source::value_type::second_type>::type result_type;

		const result_type &operator()(const typename Source::value_type &v) const
		{
			return v.second;
		}
	};

	// How to store the current value of a select.
	// If it's a computed result then store it.

	template<typename T>
	struct select_result
	{
		typedef typename std::remove_const<T>::type value_type;

		mutable util::assignable<value_type> value;

		void set_value(const value_type&v) const { value.assign(v); }
		void set_value(const value_type&&v) const { value.assign(std::move(v)); }
		const value_type & get_value() const { return *value; }
	};

	template<typename T>
	struct select_result<T&>
	{
		typedef T value_type;
		mutable const T * value;

		void set_value(const value_type &v) { value=&v; }
		const value_type & get_value() const { return *value; }
	};

	// ?? Needed ??
	template<typename T>
	struct select_result<const T&>
	{
		typedef T value_type;
		mutable const T * value;

		void set_value(const value_type &v) const { value=&v; }
		const value_type & get_value() const { return *value; }
	};

	template<typename Enumerable, typename Fn>
	class select_value_type
	{
	public:
		typedef typename util::fn_return_type<Fn,typename Enumerable::value_type>::type result_type;
		typedef typename select_result<result_type>::value_type value_type;
	};


	template<typename Enumerable, typename Fn>
	class select_t : public methods<
		select_t<Enumerable,Fn>,
		sequence<typename select_value_type<Enumerable,Fn>::value_type>>
	{
		// Ideally a lambda would define a type "result_type", like so.
		// typedef typename Fn::result_type value_type;
		// But it does not, so deduce it here.
		// typedef typeof((*(Fn*)(nullptr))(*((typename Enumerable::value_type*)nullptr))) value_type;

		// typedef typeof((*(Fn*)(nullptr))(*((typename Enumerable::value_type*)nullptr))) result_type;
		typedef typename select_value_type<Enumerable,Fn>::result_type result_type;

	public:
		typedef typename select_value_type<Enumerable,Fn>::value_type value_type;
		// typedef typename util::fn_return_type<Fn,typename Enumerable::value_type>::type result_type;

	public:
		// typedef typename Enumerable::enumerable_type enumerable_type;

		select_t(Enumerable s, Fn f) : source(s), fn(f)
		{
		}

		// typedef typename select_result<result_type>::value_type value_type;

		const value_type &get_value() const { return current.get_value(); }

		bool move_first() const
		{
			if(source.move_first())
			{
				set_value();
				return true;
			}
			else
			{
				return false;
			}
		}

		bool move_next() const
		{
			if(source.move_next())
			{
				set_value();
				return true;
			}
			else
			{
				return false;
			}
		}

		bool move_prev() const
		{
			if(source.move_prev())
			{
				set_value();
				return true;
			}
			else
			{
				return false;
			}
		}

		bool move_last() const
		{
			if(source.move_last())
			{
				set_value();
				return true;
			}
			else
			{
				return false;
			}
		}


	private:
		Enumerable source;
		Fn fn;
		select_result<result_type> current;

		void set_value() const
		{
			current.set_value( fn(source.get_value()) );
		}
	};

	template<typename Enumerable, typename Fn>
	class select_many_t : public
		methods<select_many_t<Enumerable,Fn>,
		// isequence<typename Enumerable::value_type>>
		sequence<typename select_t<Enumerable, Fn>::value_type::value_type>>

	{
		typedef select_t<Enumerable, Fn> selection_type;
	public:
		typedef typename selection_type::value_type::value_type value_type;

		select_many_t(Enumerable e, Fn fn) : selection(e, fn)
		{
		}

		const value_type & get_value() const
		{
			return selection.get_value().get_value();
		}

		bool move_first() const
		{
			for(bool valid = selection.move_first(); valid; valid=selection.move_next())
			{
				if(selection.get_value().move_first()) return true;
			}
			return false;
		}

		bool move_last() const
		{
			for(bool valid = selection.move_last(); valid; valid=selection.move_prev())
			{
				if(selection.get_value().move_last()) return true;
			}
			return false;
		}

		bool move_next() const
		{
			if(selection.get_value().move_next())
				return true;

			while(selection.move_next())
			{
				if(selection.get_value().move_first())
					return true;
			}

			return false;
		}

		bool move_prev() const
		{
			if(selection.get_value().move_prev())
				return true;

			while(selection.move_prev())
			{
				if(selection.get_value().move_last())
					return true;
			}

			return false;
		}

	private:
		selection_type selection;
	};
}

#endif
