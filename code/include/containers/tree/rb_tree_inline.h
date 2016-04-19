#pragma once

#define TEMPLATE_SPECIALIZATION template <typename K, typename V>
#define specialized_red_black_tree red_black_tree<K, V>
#define specialized_red_black_node typename specialized_red_black_tree::node_type
#define specialized_red_black_node_ptr typename specialized_red_black_tree::node_type_ptr

TEMPLATE_SPECIALIZATION specialized_red_black_node_ptr specialized_red_black_tree::leaf = singleton<specialized_red_black_node>::get_instance(); 



TEMPLATE_SPECIALIZATION void specialized_red_black_tree::balance_after_insert(node_type_ptr node)
{
	node_type_ptr parent = node->parent();
	if (parent == nullptr) 
	{
		// Case 1 - The current node is at the root of the tree.
		node->color(node_type::node_color::black);
		return;
	}
	else if (parent->is_black()) 
	{
		// Case 2 - The current node's parent is black, so property 4 (both
		// children of every red node are black) is not invalidated.
		return;
	}

	node_type_ptr grandparent = node->grandparent();
	node_type_ptr uncle = node->uncle();

	if (parent->is_red() && uncle->is_red())
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

	if (parent->is_red() && uncle->is_black())
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

	if (parent->is_red() && uncle->is_black())
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


TEMPLATE_SPECIALIZATION void specialized_red_black_tree::rotate_left(node_type_ptr node)
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

TEMPLATE_SPECIALIZATION void specialized_red_black_tree::rotate_right(node_type_ptr node)
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


TEMPLATE_SPECIALIZATION specialized_red_black_node_ptr specialized_red_black_tree::get_greatest(node_type_ptr root)
{
	node_type_ptr target = root->right();
	if (target == nullptr)
		return nullptr;
	while (target != nullptr && target != leaf)
	{
		node_type_ptr node = target->right();
		if (node != nullptr && node != leaf)
			target = node;
		else
			break;
	}
	return target;
}


TEMPLATE_SPECIALIZATION void specialized_red_black_tree::replace_node(node_type_ptr src, node_type_ptr dst)
{
	node_type_ptr left = src->left();
	node_type_ptr right = src->right();

	dst->color(src->color());
	std::memcpy(&src->_key, &dst->_key, sizeof(key_type));
	std::memcpy(&src->_value, &dst->_value, sizeof(value_type));
	dst->left(left);
	dst->right(right);

	if (left != nullptr)
		left->parent(dst);
	if (right != nullptr)
		right->parent(dst);

}

TEMPLATE_SPECIALIZATION void specialized_red_black_tree::balance_after_delete(node_type_ptr target)
{
	node_type_ptr parent = target->parent();
	if (parent == nullptr)
		return; // the target is the new root
	node_type_ptr sibling = target->sibling();
	if (sibling->is_red())
	{
		// case 2 - sibling is red
		parent->color(node_type::node_color::red);
		sibling->color(node_type::node_color::black);
		if (target == parent->left())
		{
			rotate_left(parent);
		}
		else if (target == parent->right())
		{
			rotate_right(parent);
		}
		parent = target->parent();
		sibling = target->sibling();
	}

	if (parent->is_black() && sibling->is_black() && sibling->left()->is_black() && sibling->right()->is_black())
	{
		// Case 3 - parent, sibling, and sibling's children are black.
		sibling->color(node_type::node_color::red);
		balance_after_delete(parent);
		return;
	}
	else if (parent->is_red() && sibling->is_black() && sibling->left()->is_black() && sibling->right()->is_black())
	{
		// Case 4 - sibling and sibling's children are black, but parent is red.
		parent->color(node_type::node_color::black);
		sibling->color(node_type::node_color::red);
		return;
	}
	else if (sibling->is_black())
	{
		// Case 5 - sibling is black, sibling's left child is red,
		// sibling's right child is black, and target is the left child of
		// its parent.
		if (target == parent->left() && sibling->left()->is_red() && sibling->right()->is_black())
		{
			sibling->color(node_type::node_color::red);
			rotate_right(sibling);
			parent = target->parent();
			sibling = target->sibling();
		}
		else if (target == parent->right() && sibling->left()->is_black() && sibling->right()->is_red())
		{
			sibling->color(node_type::node_color::red);
			rotate_left(sibling);
			parent = target->parent();
			sibling = target->sibling();
		}
	}

	// Case 6 - sibling is black, sibling's right child is red, and target is the left child of its parent.
	sibling->color(parent->color());
	parent->color(node_type::node_color::black);
	if (target == parent->left()) 
	{
		sibling->right()->color(node_type::node_color::black);
		rotate_left(target->parent());
	} 
	else if (target == parent->right()) 
	{
		sibling->left()->color(node_type::node_color::black);
		rotate_right(target->parent());
	} 

}

#undef specialized_red_black_node_ptr
#undef specialized_red_black_node
#undef specialized_red_black_tree
#undef TEMPLATE_SPECIALIZATION

