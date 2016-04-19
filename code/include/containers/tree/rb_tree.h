#pragma once

#include <containers/tree/node.h>
#include <containers/visitor/visitor.h>
#include <containers/proxy/proxy.h>
#include <containers/singleton/singleton.h>
#include <containers/broker/object_broker.h>
#include <cstring>
// #include <containers/tree/base_tree.h>

namespace containers 
{

	
	template <typename K, typename V> class red_black_tree //: public base_tree<rb_node>
	{	
	public:
		
		class rb_node : public node<K, V, rb_node>
		{
		public:
			friend class singleton<rb_node>;

			typedef node<K, V, rb_node> inherited;

			enum node_color 
			{
				black,
				red
			};
			
		private:
			node_color _color;

		public:
			using inherited::node;

			GENERATE_GETTER_SETTER(node_color, color)

			inline bool is_black() const
			{
				return _color == node_type::node_color::black;
			}

			inline bool is_red() const
			{
				return _color == node_type::node_color::red;
			}


		protected:
			rb_node() : _color(black)
			{
			}
		};


		typedef K key_type;
		typedef V value_type;
		//typedef base_tree<rb_node<K, V>> inherited;
		//typedef typename inherited::node_type_ptr node_type_ptr;
		//typedef typename inherited::node_type node_type;
		typedef rb_node node_type;
		typedef rb_node* node_type_ptr;
		typedef red_black_tree<key_type, value_type> self_type;

		

	private:
		node_type_ptr _root;
		size_t _size;

		static node_type_ptr leaf; 
		
	public:
		red_black_tree() : _root(nullptr), _size(0)
		{
		}

		red_black_tree(const self_type& oth) 
		{
			(*this) = oth;
		}

		red_black_tree(const self_type&& oth) 
		{
			(*this) = oth;
		}

		self_type& operator= (const self_type& oth)
		{
			destroy();
			_root = oth._root;
			_size = oth.size();
			return *this;
		}

		self_type& operator= (const self_type&& oth)
		{
			destroy();
			_root = std::move(oth._root);
			_size = std::move(oth._size);
			return *this;
		}

		virtual ~red_black_tree()
		{
			destroy();
		}

		inline void clear() 
		{
			destroy();
		}

		inline node_type_ptr root() const
		{
			return _root;
		}

		inline size_t size() const 
		{
			return _size;
		}


	public:
		inline node_type_ptr add(key_type key, value_type value)
		{
			// Case 1 - The current node is at the root of the tree.
			if (_root == nullptr)
			{
				_root = new node_type(key, value, leaf, leaf);
				_root->color(node_type::node_color::black);
				_size++;
				return _root;
			}
			// Rule 1 - Insert node like a BST would
			node_type_ptr current = _root, parent = nullptr;
			node_type_ptr target = nullptr;
			bool dir = true; // true -> left, false -> right
			while (current != nullptr) 
			{
				if (current == leaf)
				{
					target = new node_type(key, value); 
					target->color(node_type::node_color::red);
					target->left(leaf);
					target->right(leaf);
					if (dir) 
					{
						parent->left(target);
					}
					else
					{
						parent->right(target);
					}
					break;
				}
				else if ((*current) <= key)
				{
					parent = current;
					current = current->right();
					dir = false;
				}
				else 
				{
					parent = current;
					current = current->left();
					dir = true;
				}
			}
			balance_after_insert(target);
			_size++;
			return target;
		}

		inline proxy<value_type> operator[](key_type key)
		{
			node_type_ptr target = binary_search_tree(key);
			if (target != nullptr)
			{
				value_type& val = target->value();
				return proxy<value_type>(val);
			}
			return proxy<value_type>::empty;
		}

		inline void remove(key_type key)
		{
			
			node_type_ptr target = binary_search_tree(key);
			if (target == nullptr)
				return;
			// http://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html
			// https://github.com/phishman3579/java-algorithms-implementation/blob/master/src/com/jwetherell/algorithms/data_structures/RedBlackTree.java
			node_type_ptr left = target->left();
			node_type_ptr right = target->right();
			// two children
			if (left != leaf && right != leaf)
			{
				node_type_ptr greatest = get_greatest(left);
				if (greatest == nullptr || greatest == leaf)
					greatest = left;
				// Replace node with greatest in his left tree, which leaves us with only one child
				
				std::memcpy(&target->_key, &greatest->_key, sizeof(key_type));
				std::memcpy(&target->_value, &greatest->_value, sizeof(value_type));

				target = greatest;
				left = target->left();;
				right = target->right();
			}

			// one child or after memcpy
			node_type_ptr child = (left == nullptr || left == leaf) ? right : left;
			if (target->is_black())
			{
				if (child->is_black())
				{
					target->color(node_type::node_color::red);
					balance_after_delete(target);
				}
			}

			replace_node(child, target); // child data moved to target node
			// child has to be removed

			if (_root == target) 
			{
				if (_root->left() == nullptr || _root->right() == nullptr)
				{
					_root = nullptr; // we replaced the root with a leaf
				}
				else
				{
					_root->parent(nullptr);
					_root->color(node_type::node_color::black);
				}
			}
			
        	_size--;
       
		}


		inline bool contains(key_type key)
		{
			//return proxy<value_type>::empty != (*this)[key];
			return binary_search_tree(key) != nullptr;
		}


	private:

		void balance_after_delete(node_type_ptr root);

		node_type_ptr get_greatest(node_type_ptr root);
		
		void replace_node(node_type_ptr src, node_type_ptr dst);
		
		
		struct destroyer_visitor_impl : public base_visitor <void>
		{
			void operator()(node_type_ptr node)
			{
				if (node == singleton<node_type>::get_instance())
					return;
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
			destroyer_visitor_impl destroyer_visitor;
			if (_root != nullptr)
			{
				apply_visitor(destroyer_visitor, _root);
			}
		}

		inline node_type_ptr binary_search_tree(key_type& key)
		{
			node_type_ptr curr = _root;
			while (curr != nullptr)
			{
				if ((*curr) == key)
				{
					return curr;
				}
				else if ((*curr) < key)
				{
					curr = curr->right();
				}
				else
				{
					curr = curr->left();
				}
			}
			return nullptr;
		}

		void balance_after_insert(node_type_ptr node);
		

		void rotate_left(node_type_ptr node);
		

		void rotate_right(node_type_ptr node);
		
		
	};

	#include "rb_tree_inline.h"
}
