//
//  reverse.hpp
//  Linq
//
//  Created by Calum Grant on 20/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_reverse_hpp
#define Linq_reverse_hpp

namespace linq
{
	template<typename T, typename Alloc>
	class reverse_t : public container_store<std::vector<T, Alloc>>
	{
	public:
		typedef T value_type;
		
		reverse_t(const sequence<value_type> & src, Alloc alloc = Alloc()) :
			container_store<std::vector<T,Alloc>>(src, alloc)
		{
			std::reverse(this->container.begin(), this->container.end());
		}
	};
}

#endif
