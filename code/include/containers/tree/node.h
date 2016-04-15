#pragma once

namespace containers {

	template <typename K, typename V> class node 
	{

		#define GENERATE_GETTER(type, name) \
			const type name () const \
			{ \
				return _##name; \
			}

		#define GENERATE_SETTER(type, name) \
			void name (type new_value) \
			{ \
				_##name = new_value; \
			}

		#define GENERATE_GETTER_SETTER(type, name) \
			GENERATE_SETTER(type, name) \
			GENERATE_GETTER(type, name)

		typedef K key_type;
		typedef V value_type;
		typedef node<key_type, value_type> self_type;
		typedef node<key_type, value_type>* self_type_ptr;

		private:
			key_type _key;
			value_type _value;

			self_type_ptr _left;
			self_type_ptr _right;
			self_type_ptr _parent;
		public:
			node(key_type key, value_type value, self_type_ptr left = nullptr, self_type_ptr right = nullptr, self_type_ptr parent = nullptr) 
				: _key(key), _value(value), _left(left), _right(right), _parent(parent)
			{
			}

			GENERATE_GETTER_SETTER(key_type, key)
			GENERATE_GETTER_SETTER(value_type, value)
			GENERATE_GETTER_SETTER(self_type_ptr, left)
			GENERATE_GETTER_SETTER(self_type_ptr, right)
			GENERATE_GETTER_SETTER(self_type_ptr, parent)

			value_type& value() 
			{
				return _value;
			}

			friend bool operator< (const self_type& left, const self_type& right)  
			{
				return left._key < right._key;
			}

			friend bool operator< (const self_type& left, const key_type& key)  
			{
				return left._key < key;
			}

			friend bool operator== (const self_type& left, const self_type& right)  
			{
				return left._key == right._key;
			}

			friend bool operator== (const self_type& left, const key_type& key)  
			{
				return left._key == key;
			}

			friend bool operator!= (const self_type& left, const self_type& right)  
			{
				return left._key != right._key;
			}


			#undef GENERATE_GETTER_SETTER
			#undef GENERATE_SETTER
			#undef GENERATE_GETTER
	}; 

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