//
//  fwd.h
//  Linq
//
//  Created by Calum Grant on 17/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_fwd_h
#define Linq_fwd_h

namespace linq
{
	template<typename T> class isequence;

	template<typename Derived, typename Base, typename Stored=Derived> class methods;

	template<typename Source, typename Predicate>
	class where_t;

	template<typename Source, typename Predicate>
	class select_t;

	template<typename T, typename Alloc=std::allocator<T>>
	class reverse_t;

	template<typename Container>
	class container_store;

	template<typename Source>
	class skip_t;

	template<typename E1, typename E2>
	class concat_t;

	template<typename T, typename Alloc=std::allocator<T>>
	class sort_t;

	template<typename S, typename T>
	struct select_field;

	template<typename T> class limit_t;

	template<typename E, typename P> class until_t;

	template<typename E> class repeat_t;

	template<typename E, typename Fn> class select_many_t;

	template<typename E> class repeat_n_t;
	template<typename E> class keys_t;
	template<typename E> class values_t;

	template<typename T> class sequence_ptr;

	template<typename T> class sequence;
}


#endif
