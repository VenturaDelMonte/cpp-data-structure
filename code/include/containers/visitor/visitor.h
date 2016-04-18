#pragma once


namespace containers 
{

	template <class T> struct base_visitor 
	{
	public:
		typedef T return_type;
	};

	// template <class V, class T> typename V::return_type apply_visitor(V visitor, T object)
	// {
	// 	return visitor(object);
	// }

	// template <class V, class T> typename V::return_type apply_visitor(V* visitor, T object)
	// {
	// 	return (*visitor)(object);
	// }

	template <class V, class T> void apply_visitor(V visitor, T object)
	{
		visitor(object);
	}

	template <class V, class T> void apply_visitor(V* visitor, T object)
	{
		(*visitor)(object);
	}

}

