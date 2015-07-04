//
//  concat.hpp
//  Linq
//
//  Created by Calum Grant on 18/06/2015.
//  Copyright (c) 2015 Calum Grant. All rights reserved.
//

#ifndef Linq_concat_hpp
#define Linq_concat_hpp

namespace linq
{
	template<typename Enumerable1, typename Enumerable2>
	class concat_t : public v2::decorate<concat_t<Enumerable1, Enumerable2>,
		typename util::combined_interface<
			typename Enumerable1::interface_type, typename Enumerable2::interface_type>::type>
	{
	public:
		concat_t(Enumerable1 e1, Enumerable2 e2) : e1(e1), e2(e2) { }

		typedef typename Enumerable1::value_type value_type;

		bool move_first() const
		{
			if(e1.move_first())
			{
				m_first=true;
				return true;
			}
			else if(e2.move_first())
			{
				m_first=false;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool move_next() const
		{
			if(m_first)
			{
				if(e1.move_next())
				{
					return true;
				}
				else
				{
					if(e2.move_first())
					{
						m_first=false;
						return true;
					}
					else
						return false;
				}
			}
			else
			{
				return e2.move_next();
			}
		}

		bool move_last() const
		{
			if(e2.move_last())
			{
				m_first=false;
				return true;
			}
			else
			{
				if(e1.move_last())
				{
					m_first=true;
					return true;
				}
				else
					return false;
			}
		}

		bool move_prev() const
		{
			if(!m_first)
			{
				if(e2.move_prev())
				{
					return true;
				}
				else if(e1.move_last())
				{
					m_first=true;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return e1.move_prev();
			}
		}

		const value_type & get_value() const
		{
			return m_first ? e1.get_value() : e2.get_value();
		}

	private:
		Enumerable1 e1;
		Enumerable2 e2;
		mutable bool m_first;
	};
}

#endif
