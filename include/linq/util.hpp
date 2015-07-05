//
//  util.hpp
//  Linq
//
//  Created by Calum Grant on 23/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_util_hpp
#define Linq_util_hpp

namespace linq
{
	namespace util
	{
		template<typename Interface1, typename Interface2>
		struct combined_interface;

		template<typename T>
		struct combined_interface<T, T>
		{
			typedef T type;
		};

		template<typename T>
		struct combined_interface<ienumerable<T>, ireversible<T>>
		{
			typedef ienumerable<T> type;
		};

		template<typename T>
		struct combined_interface<ireversible<T>, ienumerable<T>>
		{
			typedef ienumerable<T> type;
		};

		template<typename Iterator,
			typename IteratorCategory = typename std::iterator_traits<Iterator>::iterator_category>
		struct enumerable_type;

		template<typename Iterator>
		struct enumerable_type<Iterator, std::input_iterator_tag>
		{
			typedef typename std::iterator_traits<Iterator>::value_type value_type;
			typedef ienumerable<value_type> iface;
		};

		template<typename Iterator>
		struct enumerable_type<Iterator, std::forward_iterator_tag>
		{
			typedef typename std::iterator_traits<Iterator>::value_type value_type;
			typedef ienumerable<value_type> iface;
		};

		template<typename Iterator>
		struct enumerable_type<Iterator, std::bidirectional_iterator_tag>
		{
			typedef typename std::iterator_traits<Iterator>::value_type value_type;
			typedef ireversible<value_type> iface;
		};

		template<typename Iterator>
		struct enumerable_type<Iterator, std::random_access_iterator_tag>
		{
			typedef typename std::iterator_traits<Iterator>::value_type value_type;
			typedef ireversible<value_type> iface;
		};

		template<typename Container>
		struct container_type
		{
			typedef typename enumerable_type<typename Container::const_iterator>::iface iface;
		};


		// There's probably a library for this...
		template<typename T> struct nonconst
		{
			typedef T type;
		};

		template<typename T> struct nonconst<const T>
		{
			typedef T type;
		};

		template<typename T1, typename T2>
		struct type_equal
		{
			static const bool value=false;
		};

		template<typename T>
		struct type_equal<T,T>
		{
			static const bool value=true;
		};

		template<typename T>
		struct nonref
		{
			static const bool value = false;
			typedef T type;
		};

		template<typename T>
		struct nonref<T&>
		{
			static const bool value = true;
			typedef T type;
		};

		template<typename T>
		struct is_rvalueref
		{
			static const bool value = false;
		};


		template<typename T>
		struct is_rvalueref<T&&>
		{
			static const bool value = true;
		};

		template<bool b>
		struct result
		{
			static const bool value = b;
		};

		template<typename T>
		struct is_constructible
		{
			static result<true> fn(T t);
			static result<false> fn(...);

			typedef typeof(fn(*(T*)nullptr)) t;
			static const bool value = t::value;
		};

		// Figure out the return type of a functor
		template<typename Fn, typename Arg>
		struct fn_return_type
		{
			typedef typeof( (*(Fn*)nullptr)(*(Arg*)nullptr) ) type;
		};

		// Hack to make an object which is non-assignable
		// assignable anyway (i.e. Lambdas and functors)
		template<typename T>
		class assignable
		{
			// Pragma aligned ??
			union
			{
				void *p;
				double d;
				char c[sizeof(T)];
			};

			bool is_stored;

			T * get() { return (T*)c; }
			const T * get() const { return (const T*)c; }

			void cleanup()
			{
				if(is_stored)
					get()->~T();
			}

		public:
			assignable() : is_stored(false) { }
			assignable(const T & t) : is_stored(true) { new(get()) T(t); }
			assignable(T && t) : is_stored(true) { new(get()) T(std::move(t)); }
			~assignable() { cleanup(); }

			assignable(const assignable & other) : is_stored(other.is_stored)
			{
				if(is_stored)
					new(get()) T(*other);
			}

			assignable & operator=(const assignable & other)
			{
				if(other)
					assign(*other);
				else
					reset();
			}

			assignable & operator=(assignable && other)
			{
				if(other)
					assign(std::move(*other));
				else
					reset();
			}

			T & operator*() { return *get(); }
			const T & operator*() const { return *get(); }

			void reset() { cleanup(); is_stored = false; }

			void assign(T && t)
			{
				reset();
				new(get()) T(std::move(t));
				is_stored = true;
			}

			void assign(const T & t)
			{
				reset();
				new(get()) T(t);
				is_stored = true;
			}

			operator bool() const { return is_stored; }

		};

	}
}

#endif
