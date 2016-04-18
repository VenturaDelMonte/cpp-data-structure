#pragma once

#define TEMPLATE_SPECIALIZATION template <typename T>
#define specialized_singleton singleton<T>


TEMPLATE_SPECIALIZATION T* specialized_singleton::instance = specialized_singleton::get_instance();


TEMPLATE_SPECIALIZATION T* specialized_singleton::get_instance()
{
	if (!instance)
	{
		specialized_singleton::instance = new T();
	}
	return instance;
}

TEMPLATE_SPECIALIZATION void specialized_singleton::destroy()
{
	delete specialized_singleton::instance;
	specialized_singleton::instance = nullptr;
}

#undef TEMPLATE_SPECIALIZATION
#undef specialized_singleton