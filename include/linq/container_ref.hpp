//
//  container_ref.hpp
//  Linq
//
//  Created by Calum Grant on 22/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_container_ref_hpp
#define Linq_container_ref_hpp

namespace linq
{
	// References a container
	template<typename Container>
	class container_ref : public methods<
		container_ref<Container>,
		sequence<typename Container::value_type> >
	{
	public:
		container_ref(const Container &c) : source(&c) { }
		container_ref() { }

		typedef typename Container::value_type value_type;

		bool move_first() const { current = source->begin(); return current!=source->end(); }
		bool move_next() const { return ++current != source->end(); }

		const value_type & get_value() const { return *current; }

	private:
		const Container * source;
		mutable typename Container::const_iterator current;
	};


	template<typename C>
	container_ref<C> from(const C &c)
	{
		return container_ref<C>(c);
	}

	template<typename T>
	container_ref<std::initializer_list<T>> from(const std::initializer_list<T> & c)
	{
		return container_ref<std::initializer_list<T>>(c);
	}
}

#endif
