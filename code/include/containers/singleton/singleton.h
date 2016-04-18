#pragma once

namespace containers 
{

	template <typename T> class singleton
	{
	public:
		static T* get_instance();
		static void destroy();

	private:
		singleton(singleton const&) 
		{
		}

		singleton& operator= (singleton const&)
		{
		}

	protected:
		static T* instance;

		singleton() 
		{
			instance = static_cast<T*>(this);
		}

		virtual ~singleton()
		{
		}

	};


	#include "singleton_inline.h"

}
