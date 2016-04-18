#pragma once

#include <cstdint>
#include <algorithm>
#include <vector>
#include <functional>
#include <containers/visitor/visitor.h>
#include <containers/tree/node.h>
#include <containers/proxy/proxy.h>
#include <containers/broker/object_broker.h>
#include <stdexcept>
#include <type_traits>

namespace containers 
{

	template <typename K, typename V, int DEFAULT_SIZE = 32> class hashmap 
	{
	public:
		class hashmap_node;
		class hashmap_node : public node<K, V, hashmap_node>
		{
			public:
				typedef node<K, V, hashmap_node> inherited;
				typedef hashmap_node self_type;
				typedef self_type* self_type_ptr;

				using inherited::node;
		};

		typedef K key_type;
		typedef V value_type;
		typedef hashmap_node bucket_type;
		typedef hashmap_node* bucket_type_ptr;
		typedef std::vector<bucket_type_ptr> bucket_array_type;
		typedef typename bucket_array_type::iterator bucket_array_iterator;
		typedef hashmap<key_type, value_type, DEFAULT_SIZE> self_type;
		typedef std::hash<key_type> hash_function_type;

		

	private:

		bucket_array_type _buckets;
		uint32_t _size;


	public:

		hashmap() : _size(0)
		{
			_buckets.resize(DEFAULT_SIZE);
			std::fill(_buckets.begin(), _buckets.end(), nullptr);
		}

		hashmap(const self_type& oth) 
		{
			(*this) = oth;
		}

		hashmap(const self_type&& oth) 
		{
			(*this) = oth;
		}

		inline self_type& operator= (const self_type& oth)
		{
			destroy();
			std::copy(oth._buckets.begin(), oth._buckets.end(), _buckets.begin());
			_size = oth.size();
			return *this;
		}

		inline self_type& operator= (const self_type&& oth)
		{
			destroy();
			_buckets = std::move(oth._buckets);
			_size = std::move(oth._size);
			return *this;
		}

		virtual ~hashmap()
		{
			destroy();
		}

		inline void clear() 
		{
			destroy();
		}

		inline uint32_t size() const 
		{
			return _size;
		}

		inline void add(key_type key, value_type value)
		{
			size_t key_hash = hash(key);
			bucket_type_ptr root = _buckets[key_hash];
			if (root == nullptr)
			{
				_buckets[key_hash] = new bucket_type(key, value);
			}
			else
			{
				while (root != nullptr)
				{
					if ((*root) == key)
					{
						update_helper<value_type>::destroy(root->value());
						root->value(value);
						break;
					}
					else if ((*root) < key) // key is bigger so it goes to right
					{
						if (root->right() == nullptr)
						{
							root->right(new bucket_type(key, value));
							root->right()->parent(root);
							_size++;
							break;
						}
						else
						{
							root = root->right();
						}
					}
					else
					{
						if (root->left() == nullptr)
						{
							root->left(new bucket_type(key, value));
							root->left()->parent(root);
							_size++;
							break;
						}
						else
						{
							root = root->left();
						}
					}
				}
			}
		}

/*
		value_type operator[](key_type key)
		{
			bucket_type_ptr target = binary_search_tree(key);
			if (target != nullptr)
			{
				return target->value();
			}
			throw std::out_of_range(std::string("cannot find key: ") + key);
		}
*/
		
		inline proxy<value_type> operator[](key_type key)
		{
			bucket_type_ptr target = binary_search_tree(key);
			if (target != nullptr)
			{
				value_type& val = target->value();
				return proxy<value_type>(val);
			}
			return proxy<value_type>::empty;
		}

    private:
    	
    	
		struct visitor_impl : public base_visitor<void>
		{
			void operator()(bucket_type_ptr node)
			{
				if (node->left() != nullptr)
				{
					apply_visitor(this, node->left());
					node->left(nullptr);
				}
				if (node->right() != nullptr)
				{
					apply_visitor(this, node->right());
					node->right(nullptr);
				}
				update_helper<value_type>::destroy(node->value());
				delete node;
			}
		};
    	

    	void destroy()
		{
			visitor_impl visitor;
			for (auto root : _buckets)
			{
				if (root != nullptr)
				{
					apply_visitor(visitor, root);
				}
			}
		}

		inline size_t hash(key_type& key) 
		{
			hash_function_type hash_helper;
			size_t ret = hash_helper(key);
			ret >>= 16;
			ret = ret % DEFAULT_SIZE;
			return ret;
		}

		inline bucket_type_ptr binary_search_tree(key_type& key)
		{
			size_t key_hash = hash(key);
			bucket_type_ptr root = _buckets[key_hash];
			while (root != nullptr)
			{
				if ((*root) == key)
				{
					return root;
				}
				else if ((*root) < key)
				{
					root = root->right();
				}
				else
				{
					root = root->left();
				}
			}
			return nullptr;
		}

	}; 


	

}