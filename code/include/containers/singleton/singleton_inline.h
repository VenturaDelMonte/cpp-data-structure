#pragma once

#define TEMPLATE_SPECIALIZATION template <typename T>
#define specialized_singleton singleton<T>


TEMPLATE_SPECIALIZATION T* specialized_singleton::instance = specialized_singleton::get_instance();


TEMPLATE_SPECIALIZATION T* specialized_singleton::get_instance()
{
	static T t;
	specialized_singleton::instance = &t;
	return specialized_singleton::instance;
}


#undef TEMPLATE_SPECIALIZATION
#undef specialized_singleton