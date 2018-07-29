#include "ibst.h"

template <typename K, typename V>
BST<K, V>::~BST() {
	unsigned int n_nodes;
	Node ** nodes = Serialize(&n_nodes);
	if (n_nodes > 0) {
		for (unsigned int i = 0; i < n_nodes; i++) {
			delete nodes[i];
		}
		delete[] nodes;
	}
}

template <typename K, typename V>
typename BST<K, V>::Node * BST<K, V>::Insert(K key, V value) {
	Node * new_node = new Node();
	new_node->key = key;
	new_node->value = value;
	Node * result = Attach(new_node);
	
	if (!result) {
		delete new_node;
		return NULL;
	}
	else {
		n_nodes++;
		return result;
	}
}

template <typename K, typename V>
typename BST<K, V>::Node * BST<K, V>::Find(K key) {
	if (!root) {
		return NULL;
	}
	
	Node * parent = root;
	Node * child = root;
	int cmp_result;
	do {
		parent = child;
		cmp_result = compare_func(key, parent->GetKey());
		
		if (cmp_result == 0) {
			return parent;
		}
		
		child = cmp_result < 0 ? parent->left : parent->right;
	}
	while (child);
	
	return NULL;
}

template <typename K, typename V>
typename BST<K, V>::Node * BST<K, V>::First() {
	if (!root) {
		return NULL;
	}
	
	Node * parent = root;
	Node * child = root;
	do {
		parent = child;
		child = parent->left;
	}
	while (child);
	
	return parent;
}

template <typename K, typename V>
typename BST<K, V>::Node * BST<K, V>::Last() {
	if (!root) {
		return NULL;
	}
	
	Node * parent = root;
	Node * child = root;
	do {
		parent = child;
		child = parent->right;
	}
	while (child);
	
	return parent;
}

template <typename K, typename V>
typename BST<K, V>::Node ** BST<K, V>::Serialize(unsigned int * n_nodes) {
	
	// Check for empty BST
	if (!root) {
		*n_nodes = 0;
		return NULL;
	}
	
	// Allocate output memory
	Node ** output = new Node *[this->n_nodes];
	unsigned int output_i = 0;
	
	// Create stack
	Node ** stack = new Node *[max_depth + 1];
	stack[1] = root;
	unsigned int stack_i = 1;
	
	Node * last = Last();
	
	// Go to first value on extreme left
	while (stack[stack_i]->left) {
		stack[stack_i + 1] = stack[stack_i]->left;
		stack_i += 1;
	}
	
	do {
		// Add node to output
		output[output_i++] = stack[stack_i];
		
		// Exit loop if last node
		if (stack[stack_i] == last) break;
		
		// The node has a right child
		if (stack[stack_i]->right) {
			// Go right 1, and down as far left as possible
			stack[stack_i + 1] = stack[stack_i]->right;
			stack_i += 1;
			while (stack[stack_i]->left) {
				stack[stack_i + 1] = stack[stack_i]->left;
				stack_i += 1;
			}
		}
		
		// The node is a right child but does not have a right child
		else if (stack[stack_i] == stack[stack_i - 1]->right) {
			// Go up until a left child node is found
			while (stack_i > 2 && stack[stack_i] == stack[stack_i - 1]->right) {
				stack_i -= 1;
			}
			stack_i -= 1;
		}
		
		// The node is a left child but does not have a right child
		else {
			// Go up 1
			stack_i -= 1;
		}
	}
	while (1);
		
	delete[] stack;
	
	*n_nodes = output_i;
	return output;
}

template <typename K, typename V>
BST<K, V> * BST<K, V>::Rebalance() {
	
	// Get serialized nodes
	unsigned int n_nodes;
	Node ** nodes = Serialize(&n_nodes);
	
	if (n_nodes == 0) return NULL;
	
	// Get MSB
	unsigned int filter = 1;
	for (int i = 0; i < 32 && filter <= n_nodes / 2; i++) {
		filter = filter << 1;
	}
	
	BST * output = new BST();
	output->compare_func = compare_func;
	while (filter) {
		for (unsigned int index = filter - 1; index < n_nodes; index += filter * 2) {
			output->Insert(nodes[index]->key, nodes[index]->value);
		}
		filter = filter >> 1;
	}
	
	delete[] nodes;
	
	return output;
}

template <typename K, typename V>
typename BST<K, V>::Node * BST<K, V>::Attach(BST::Node * node) {
	if (!node) {
		return NULL;
	}
	if (!root) {
		root = node;
		max_depth = 1;
		return node;
	}
	
	Node * parent = root;
	Node * child = root;
	int cmp_result;
	unsigned int depth = 0;
	
	do {
		parent = child;
		depth++;
		
		cmp_result = compare_func(node->GetKey(), parent->GetKey());
		if (cmp_result == 0) {
			return NULL;
		}
		child = cmp_result < 0 ? parent->left : parent->right;
	}
	while (child);
	
	if (cmp_result < 0) {
		parent->left = node;
	}
	else {
		parent->right = node;
	}
	if (max_depth < depth + 1) {
		max_depth = depth + 1;
	}
	return node;
}

template class BST<int, int>;