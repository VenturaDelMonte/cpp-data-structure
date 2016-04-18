#pragma once

namespace containers 
{

	template <typename T> class base_tree 
	{
	protected:
		size_t m_size;
	public:
		typedef T node_type;
		typedef T* node_type_ptr;

		base_tree() : m_size(0)
		{

		}

		virtual ~base_tree() 
		{
	 	}

	 	inline virtual node_type_ptr root() const = 0;

	 	inline virtual size_t size() const = 0;
	 	
	};

}