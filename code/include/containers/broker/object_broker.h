#pragma once

template <class T> class update_helper
{
public:
	static void destroy(T ptr) {}
};

template <class T> class update_helper<T*> 
{
public:
	static void destroy(T ptr)
	{
		delete ptr;
	}
};
