//
//  sort.hpp
//  Linq
//
//  Created by Calum Grant on 22/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_sort_hpp
#define Linq_sort_hpp

namespace linq
{
	template<typename T, typename Alloc>
	class sort_t : public container_store<std::vector<T, Alloc>>
	{
	public:
		typedef T value_type;
		
		template<typename Cmp>
		sort_t(const sequence<value_type> & src, Cmp cmp=std::less<value_type>(), Alloc alloc = Alloc()) : container_store<std::vector<T,Alloc>>(src, alloc)
		{
			std::sort(this->container.begin(), this->container.end(), cmp);
		}
	};
}

#endif
