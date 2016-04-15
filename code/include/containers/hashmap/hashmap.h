#pragma once

#include <cstdint>
#include <algorithm>
#include <vector>
#include <functional>
#include <containers/tree/node.h>
#include <stdexcept>
#include <type_traits>

namespace containers 
{
	
	template <typename reference_type> class proxy 
	{
		public:
			proxy(reference_type& r) : holder(&r) {}
			void operator= (reference_type& val) 
			{
				*holder = val;
			}	
			void operator= (reference_type&& val) 
			{
				*holder = std::move(val);
			}	
			operator reference_type() 
			{
				return *holder;
			}
		private:
			reference_type* holder;
	};

	template <typename K, typename V, int DEFAULT_SIZE = 32> class hashmap 
	{
	public:
		typedef K key_type;
		typedef V value_type;
		typedef node<key_type, value_type> bucket_type;
		typedef node<key_type, value_type>* bucket_type_ptr;
		typedef std::vector<bucket_type_ptr> bucket_array_type;
		typedef typename bucket_array_type::iterator bucket_array_iterator;
		typedef hashmap<key_type, value_type, DEFAULT_SIZE> self_type;
		typedef std::hash<key_type> hash_function_type;

	private:

		bucket_array_type _buckets;
		uint32_t _size;


	public:

		hashmap() 
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
			(*this)(std::move(oth));
		}

		self_type& operator= (const self_type& oth)
		{
			destroy();
			std::copy(oth._buckets.begin(), oth._buckets.end(), _buckets.begin());
			_size = oth.size();
			return *this;
		}

		self_type& operator= (const self_type&& oth)
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

		void clear() 
		{
			
		}

		uint32_t size() const 
		{
			return _size;
		}

		void add(key_type key, value_type value)
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
					else if ((*root) < key)
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
					else
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
		
		proxy<value_type> operator[](key_type key)
		{
			bucket_type_ptr target = binary_search_tree(key);
			if (target != nullptr)
			{
				value_type& val = target->value();
				return proxy<value_type>(val);
			}
			throw std::out_of_range(std::string("cannot find key: ") + key);
		}

    private:
    	void destroy()
		{
			for (auto root : _buckets)
			{
				if (root != nullptr)
				{
					struct visitor_impl : public base_visitor<void>
					{
						void operator()(bucket_type_ptr node)
						{
							if (node->left() != nullptr)
							{
								apply_visitor(this, node->left());
								delete node->left();
								node->left(nullptr);
							}
							if (node->right() != nullptr)
							{
								apply_visitor(this, node->right());
								delete node->right();
								node->right(nullptr);
							}
							update_helper<value_type>::destroy(node->value());

						}
					} visitor;
					apply_visitor(visitor, root);
				}
			}
		}

		size_t hash(key_type& key) 
		{
			hash_function_type hash_helper;
			size_t ret = hash_helper(key);
			ret >>= 16;
			ret = ret % DEFAULT_SIZE;
			return ret;
		}

		bucket_type_ptr binary_search_tree(key_type& key)
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
					root = root->left();
				}
				else
				{
					root = root->right();
				}
			}
			return nullptr;
		}

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




	}; 


	

}