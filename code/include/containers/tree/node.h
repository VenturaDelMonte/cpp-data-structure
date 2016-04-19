#pragma once

namespace containers {


	#define GENERATE_GETTER(type, name) \
		inline const type name () const \
		{ \
			return _##name; \
		}

	#define GENERATE_SETTER(type, name) \
		inline void name (const type new_value) \
		{ \
			_##name = new_value; \
		} \

	#define GENERATE_GETTER_SETTER(type, name) \
		GENERATE_SETTER(type, name) \
		GENERATE_GETTER(type, name)

	template <typename K, typename V, typename M> class node 
	{
		public:
			typedef K key_type;
			typedef V value_type;
			typedef M self_type;
			typedef self_type* self_type_ptr;

		public:
			key_type _key;
			value_type _value;

			self_type_ptr _left;
			self_type_ptr _right;
			self_type_ptr _parent;
		public:
			explicit node(key_type key, value_type value, self_type_ptr left = nullptr, self_type_ptr right = nullptr, self_type_ptr parent = nullptr) 
				: _key(key), _value(value), _left(left), _right(right), _parent(parent)
			{
			}

			GENERATE_GETTER_SETTER(key_type, key)
			GENERATE_GETTER_SETTER(value_type, value)
			GENERATE_GETTER_SETTER(self_type_ptr, left)
			GENERATE_GETTER_SETTER(self_type_ptr, right)
			GENERATE_GETTER_SETTER(self_type_ptr, parent)

			inline self_type_ptr grandparent() const
			{
				return (_parent != nullptr) ? _parent->parent() : nullptr;
			}

			inline self_type_ptr uncle() const
			{
				self_type_ptr gf = grandparent();
				return (gf != nullptr) ? ((gf->left() == _parent) ? gf->right() : gf->left()) : nullptr;
			}

			inline self_type_ptr sibling() const
			{
				return (_parent != nullptr) ? ((_parent->left() == this) ? _parent->right() : ((_parent->right() == this) ? _parent->left() : nullptr)) : nullptr;
			}

			inline value_type& value() 
			{
				return _value;
			}

			inline friend bool operator< (const self_type& left, const self_type& right)  
			{
				return left._key < right._key;
			}

			inline friend bool operator<= (const self_type& left, const self_type& right)  
			{
				return left._key <= right._key;
			}

			inline friend bool operator< (const self_type& left, const key_type& key)  
			{
				return left._key < key;
			}

			inline friend bool operator<= (const self_type& left, const key_type& key)  
			{
				return left._key <= key;
			}

			inline friend bool operator== (const self_type& left, const self_type& right)  
			{
				return left._key == right._key;
			}

			inline friend bool operator== (const self_type& left, const key_type& key)  
			{
				return left._key == key;
			}

			inline friend bool operator!= (const self_type& left, const self_type& right)  
			{
				return left._key != right._key;
			}
		protected:
			node() {}

	}; 

	

	

}