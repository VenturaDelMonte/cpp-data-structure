#pragma once

#include <containers/singleton/singleton.h>

namespace containers
{
	template <typename T> class proxy 
	{
		//friend class singleton<proxy<reference_type>>;
		
		public:
			typedef T reference_type;
			typedef proxy<reference_type> self_type;

			proxy(reference_type& r) : holder(&r) {}

			proxy(reference_type* r = nullptr) : holder(r) {}

			inline self_type& operator= (reference_type& val) 
			{
				*holder = val;
				return *this;
			}	

			inline self_type& operator= (reference_type&& val) 
			{
				*holder = std::move(val);
				return *this;
			}	

			inline operator reference_type() 
			{
				return (holder != nullptr) ? *holder : 0;
			}

			inline reference_type operator* () const
			{
				return (holder != nullptr) ? *holder : 0;
			}

		protected:
			reference_type* holder;
		public:
			static proxy<reference_type>& empty;


			inline friend bool operator== (const self_type& left, const self_type& right)
			{
				return left.holder == right.holder;
			}

			inline friend bool operator!= (const self_type& left, const self_type& right)
			{
				return left.holder != right.holder;
			}
	};


	template <class reference_type> proxy<reference_type>& proxy<reference_type>::empty = *(singleton<proxy<reference_type>>::get_instance());
}
