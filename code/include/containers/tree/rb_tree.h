#pragma once

#include <containers/tree/node.h>
#include <containers/visitor/visitor.h>
#include <containers/proxy/proxy.h>
#include <containers/singleton/singleton.h>
#include <containers/broker/object_broker.h>

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
				red,
				black
			};
			
		private:
			node_color _color;
		public:
			using inherited::node;

			GENERATE_GETTER_SETTER(node_color, color)

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
			node_type* leaf = singleton<rb_node>::get_instance(); //const_cast<node_type*>(&dummy);
			// Case 1 - The current node is at the root of the tree.
			if (_root == nullptr)
			{
				_root = new node_type(key, value);
				_root->left(leaf);
				_root->right(leaf);
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

	private:
		
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

		void balance_after_insert(node_type_ptr node)
		{
			node_type_ptr parent = node->parent();
			if (parent == nullptr) 
			{
				// Case 1 - The current node is at the root of the tree.
				node->color(node_type::node_color::black);
				return;
			}
			else if (parent->color() == node_type::node_color::black) 
			{
				// Case 2 - The current node's parent is black, so property 4 (both
				// children of every red node are black) is not invalidated.
				return;
			}

			node_type_ptr grandparent = node->grandparent();
			node_type_ptr uncle = node->uncle();

			if (parent->color() == node_type::node_color::red && uncle->color() == node_type::node_color::red)
			{	
				// Case 3 - If both the parent and the uncle are red, then both of
				// them can be repainted black and the grandparent becomes
				// red (to maintain property 5 (all paths from any given node to its
				// leaf nodes contain the same number of black nodes)).
				parent->color(node_type::node_color::black);
				uncle->color(node_type::node_color::black);
				grandparent->color(node_type::node_color::red);
				balance_after_insert(grandparent);
				return;
			}

			if (parent->color() == node_type::node_color::red && uncle->color() == node_type::node_color::black)
			{	
				// Case 4 - The parent is red but the uncle is black; also, the
            	// current node is the right child of parent, and parent in turn
            	// is the left child of its parent grandparent.
            	if (node == parent->right() && parent == grandparent->left())
            	{
					rotate_left(parent);
					node = node->left();
					parent = node->parent();
					grandparent = node->grandparent();
					uncle = node->uncle();
            	}
            	else if (node == parent->left() && parent == grandparent->right())
            	{
            		rotate_right(parent);
            		node = node->right();
					parent = node->parent();
					grandparent = node->grandparent();
					uncle = node->uncle();
            	}
			}

			if (parent->color() == node_type::node_color::red && uncle->color() == node_type::node_color::black)
			{	
				// Case 5 - The parent is red but the uncle is black, the
				// current node is the left child of parent, and parent is the
				// left child of its parent G.
				parent->color(node_type::node_color::red);
				parent->color(node_type::node_color::red);
				if (node == parent->left() && parent == grandparent->left())
				{
					rotate_right(grandparent);
				} 
				else if (node == parent->right() && parent == grandparent->right())
				{
					rotate_left(grandparent);
				}
			}
		}

		void rotate_left(node_type_ptr node)
		{
			/*
			    r			   r
			    \			    \
				x	    =>		y							
			   / \			   / \	
			  a   y 		  x   c
			  	 | \		 / \
			  	 b  c       a   b    
			*/
			node_type_ptr parent = node->parent();
			node_type_ptr greater = node->right();
			node_type_ptr lesser = greater->left();

			greater->left(node);
			node->parent(greater);
			node->right(lesser);

			if (lesser != nullptr)
			{
				lesser->parent(node);
			}
			
			if (parent != nullptr) 
			{
				if (node == parent->left())
				{
					parent->left(greater);
				} 
				else if (node == parent->right())
				{
					parent->right(greater);
				} 
				greater->parent(parent);
			} 
			else 
			{
				_root = greater;
				_root->parent(nullptr);
			}
		}

		void rotate_right(node_type_ptr node)
		{
			/*
			    r			   r
			    \			    \
				x	    =>		y							
			   / \			   / \	
			  y   c 		  a   x
			 | \		 		 / \
			 a  b       		b   c    
			*/
			node_type_ptr parent = node->parent();
			node_type_ptr lesser = node->left();
			node_type_ptr greater = lesser->right();

			lesser->right(node);
			node->parent(lesser);
			node->left(greater);

			if (lesser != nullptr)
			{
				lesser->parent(node);
			}
			
			if (parent != nullptr) 
			{
				if (node == parent->left())
				{
					parent->left(lesser);
				} 
				else if (node == parent->right())
				{
					parent->right(lesser);
				} 
				lesser->parent(parent);
			} 
			else 
			{
				_root = lesser;
				_root->parent(nullptr);
			}
		}
		
	};
}
