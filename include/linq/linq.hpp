//
//  linq.hpp
//  Linq
//
//  Created by Calum Grant on 17/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_linq2_hpp
#define Linq_linq2_hpp

// #include <algorithm>

namespace linq
{
	// !! Think about this
	template<typename Derived, typename Base, typename Generic>
	struct user_hooks
	{
	};

	namespace v2
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
		class ireversible : public ienumerable<T>
		{
		public:
			virtual bool move_prev() const=0;
			virtual bool move_last() const=0;

			typedef ireversible<T> interface_type;
			typedef std::bidirectional_iterator_tag iterator_category;
		};


		template<typename T>
		class enumerable : public decorate<enumerable<T>, ienumerable<T>>
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

		template<typename T>
		class reversible : public decorate<reversible<T>, ireversible<T>>
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
	}

	template<typename T>
	std::ostream & operator<<(std::ostream & os, const v2::ienumerable<T> &e)
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


	#if 0

	// Decorates any container with linq methods
	// !! Remove this class !!!
	template<typename Enumerable>
	class linq :
		public v2::decorate<linq<Enumerable>, typename Enumerable::interface_type>
	{
	public:
	public:
		linq(Enumerable s) : source(s) { }

		typedef typename Enumerable::value_type value_type;

		template<typename P>
		linq<::linq::where<Enumerable, P>> where(P pred) const
		{
			return ::linq::where<Enumerable,P>(source,pred);
		}

		// std::vector<value_type> vector() const;

		//linq<store<std::vector<value_type>>> copy() const
		//{
		//	return store<std::vector<value_type>>(source);
		//}

		//linq<typename ::linq::reverse<Enumerable>::type> reverse() const;

		linq<skip<Enumerable>> skip(int n) const
		{
			return ::linq::skip<Enumerable>(source,n);
		}



		template<typename E2>
		linq<::linq::concat<Enumerable, E2>> concat(const linq<E2> & e2) const
		{
			return ::linq::concat<Enumerable, E2>(source, e2.source);
		}

		linq<::linq::reverse<Enumerable>> reverse() const
		{
			return ::linq::reverse<Enumerable>(source);
		}

		linq<::linq::limit<Enumerable>> first(int n) const
		{
			return ::linq::limit<Enumerable>(source, n);
		}

		linq<::linq::skip<Enumerable>> last(int n) const
		{
			return ::linq::skip<Enumerable>(source, this->count()-n);
		}

		typedef const value_type & const_reference;

		const_reference get_value() const { return source.get_value(); }
		bool move_first() const { return source.move_first(); }
		bool move_next() const { return source.move_next(); }
		bool move_prev() const { return source.move_prev(); }
		bool move_last() const { return source.move_last(); }

		template<typename Predicate>
		linq<::linq::until<Enumerable,Predicate>> until(Predicate p) const
		{
			return ::linq::until<Enumerable,Predicate>(source, p);
		}

		template<typename Fn>
		linq<::linq::select_many<Enumerable,Fn>> select_many(Fn fn) const
		{
			return ::linq::select_many<Enumerable,Fn>(source, fn);
		}

		linq<::linq::repeat<Enumerable>> repeat() const
		{
			return ::linq::repeat<Enumerable>(source);
		}

		linq<::linq::repeat_n<Enumerable>> repeat(int n) const
		{
			return ::linq::repeat_n<Enumerable>(source, n);
		}

	private:
	public:
		Enumerable source;
	};
	#endif

}


#endif
