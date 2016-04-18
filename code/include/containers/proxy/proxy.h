#pragma once

#include <containers/singleton/singleton.h>

namespace containers
{
	template <typename reference_type> class proxy 
	{
		//friend class singleton<proxy<reference_type>>;
		public:
			proxy(reference_type& r) : holder(&r) {}
			proxy(reference_type* r = nullptr) : holder(r) {}
			void operator= (reference_type& val) 
			{
				*holder = val;
			}	
			void operator= (reference_type&& val) 
			{
				*holder = std::move(val);
			}	
			operator reference_type() 
			{
				return (holder != nullptr) ? *holder : 0;
			}
		private:
			reference_type* holder;
		public:
			static proxy<reference_type>& empty;
	};


	template <class reference_type> proxy<reference_type>& proxy<reference_type>::empty = *(singleton<proxy<reference_type>>::get_instance());
}
