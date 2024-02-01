/*******************************************************************************
 * Name        : rbtree.h
 * Author      : Alisha Patel, Birva Patel
 * Version     : 1.0
 * Date        : 11/17/22
 * Description : Implementation of red-black tree.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#ifndef RBTREE_H_
#define RBTREE_H_

#include "node.h"
#include "tree.h"
#include "treeprinter.h"
#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>





// Forward declaration
template <typename K, typename V>
class RedBlackTree;


/**
 * tree_exception class
 * Demonstrates how you can write your own custom exceptions in C++.
 */
class tree_exception: public std::exception {
public:
    tree_exception(const std::string &message) : message_{message} { }

    ~tree_exception() throw() { }

    virtual const char* what() const throw() {
        return message_.c_str();
    }

private:
    std::string message_;
};

template <typename K, typename V>
class RedBlackTreeIterator {
public:
    /**
     * Constructor
     */
    RedBlackTreeIterator() : node_ptr{nullptr}, tree{nullptr} { }

    /**
     * Equality operator. Compares node pointers.
     */
    bool operator==(const RedBlackTreeIterator &rhs) const {
        return node_ptr == rhs.node_ptr;
    }

    /**
     * Inequality operator. Compares node pointers.
     */
    bool operator!=(const RedBlackTreeIterator &rhs) const {
        return node_ptr != rhs.node_ptr;
    }

    /**
     * Dereference operator. Returns a reference to the Node pointed to
     * by node_ptr.
     */
    Node<K, V>& operator*() const {
        return *node_ptr;
    }

    /**
     * Dereference operator. Returns a pointer to the Node pointed to
     * by node_ptr.
     */
    Node<K, V>* operator->() const {
        return node_ptr;
    }

    /**
     * Preincrement operator. Moves forward to next larger value.
     */
    RedBlackTreeIterator& operator++() {
        Node<K, V> *p;

        if (node_ptr == nullptr) {
            // ++ from end(). Get the root of the tree.
            node_ptr = tree->root_;

            // Error, ++ requested for an empty tree.
            if (node_ptr == nullptr)
                throw tree_exception(
                        "RedBlackTreeIterator operator++(): tree empty");

            // Move to the smallest value in the tree, which is the first node
            // in an inorder traversal.
            while (node_ptr->left != nullptr) {
                node_ptr = node_ptr->left;
            }
        } else {
            if (node_ptr->right != nullptr) {
                // Successor is the leftmost node of right subtree.
                node_ptr = node_ptr->right;

                while (node_ptr->left != nullptr) {
                    node_ptr = node_ptr->left;
                }
            } else {
                // Have already processed the left subtree, and
                // there is no right subtree. Move up the tree,
                // looking for a parent for which node_ptr is a left child,
                // stopping if the parent becomes nullptr (or in this case,
                // root_parent_. A non-null parent is the successor. If parent
                // is nullptr, the original node was the last node inorder, and
                // its successor is the end of the list.
                p = node_ptr->parent;
                while (p != nullptr && node_ptr == p->right) {
                    node_ptr = p;
                    p = p->parent;
                }

                // If we were previously at the rightmost node in
                // the tree, node_ptr = nullptr, and the iterator specifies
                // the end of the list.
                node_ptr = p;
            }
        }

        return *this;
    }

    /**
     * Postincrement operator. Moves forward to next larger value.
     */
    RedBlackTreeIterator operator++(int) {
        RedBlackTreeIterator tmp(*this);
        operator++(); // prefix-increment this instance
        return tmp;   // return value before increment
    }

private:
    // node_ptr is the current location in the tree. We can move
    // freely about the tree using left, right, and parent.
    // tree is the address of the RedBlackTree object associated
    // with this iterator. It is used only to access the
    // root pointer, which is needed for ++ and --
    // when the iterator value is end().
    Node<K, V> *node_ptr;
    RedBlackTree<K, V> *tree;
    friend class RedBlackTree<K, V>;

    /**
     * Constructor used to construct an iterator return value from a tree
     * pointer.
     */
    RedBlackTreeIterator(Node<K, V> *p, RedBlackTree<K, V> *t) :
        node_ptr(p), tree(t) { }
};

template<typename K, typename V>
class RedBlackTree : public Tree {
public:
    typedef RedBlackTreeIterator<K, V> iterator;

    /**
     * Constructor to create an empty red-black tree.
     */
    explicit RedBlackTree() : root_{nullptr}, size_{0} { }

    /**
     * Constructor to create a red-black tree with the elements from the
     * vector.
     */
    explicit RedBlackTree(std::vector< std::pair<K, V> > &elements) :
                                            root_(nullptr), size_(0) {
        insert_elements(elements);
    }

    /**
     * Destructor.
     */
    ~RedBlackTree() {
        delete_tree(root_);
    }

    /**
     * Inserts elements from the vector into the red-black tree.
     * Duplicate elements are not inserted.
     */
    void insert_elements(std::vector< std::pair<K, V> > &elements) {
        for (size_t i = 0, len = elements.size(); i < len; ++i) {
            try {
                insert(elements[i].first, elements[i].second);
            } catch (const tree_exception &te) {
                std::cerr << "Warning: " << te.what() << std::endl;
            }
        }
    }

    /**
     * Inserts a key-value pair into the red black tree.
     * const iterator &it indicates where to start the search for the place to
     * insert the node. If it == end(), the search starts at the root.
     * Must throw a tree_exception if attempting to insert a key that is
     * already present in the tree.
     */
    void insert(const iterator &it, const std::pair<K, V> &key_value) {
        const K& key = key_value.first;
        Node<K, V> *x, *y;
        if (it != end()) {
            x = it.node_ptr;
            y = x->parent;
        } else {
            x = root_;
            y = nullptr;
        }
        // TODO
        //if the key is already present in the tree; throw a key_exception error
        if (find(key) != end()) {
        	std::ostringstream iss;
        	iss << "Attempt to insert duplicate key '" << key << "'.";
        	throw tree_exception(iss.str());
        }

        //else insert the key in right place based in the pseudocode of CLRS
        Node<K, V> *z = new Node<K, V>(key, key_value.second);
        while (x != nullptr) {
        	y = x;
        	if (x->key() > z->key()) {
        		x = x->left;
        	} else {
        		x = x->right;
        	}
        }
        z->parent = y;
        if (y == nullptr) {
        	root_ = z;
        }
        else if (y->key() > z->key()) {
        	y->left = z;
        }
        else {
        	y->right = z;
        }
        z->left = nullptr;
        z->right = nullptr;
        z->color = RED;
        insert_fixup(z);
        size_++;

    }

    /**
     * Inserts a key-value pair into the red-black tree.
     */
    void insert(const K &key, const V &value) {
        iterator e = end();
        insert(e, std::pair<K, V>(key, value));
    }

    /**
     * Returns an ASCII representation of the red-black tree.
     */
    std::string to_ascii_drawing() {
        BinaryTreePrinter<K, V> printer(root_);
        return printer.to_string();
    }

    /**
     * Returns the height of the red-black tree.
     */
    int height() const {
        return height(root_);
    }

    /**
     * Returns the number of nodes in the red-black tree.
     */
    size_t size() const {
        return size_;
    }

    /**
     * Returns the leaf count of the red-black tree.
     */
    size_t leaf_count() const {
        return leaf_count(root_);
    }

    /**
     * Returns the internal node count of the red-black tree.
     */
    size_t internal_node_count() const {
        return internal_node_count(root_);
    }

    /**
     * Returns the diameter of the red-black tree rooted at node.
     * Diameter is defined as the length of the longest path between
     * two (non-null) leaves in the tree. The path does not necessarily have to
     * pass through the root.
     */
    size_t diameter() const {
        // TODO
    	//returns the diameter of the tree based on the root
    	//calls the helper function
    	return diameter(root_);
    }

    /**
     * Returns the max width of the red-black tree, i.e. the largest number of
     * nodes on any level.
     */
    size_t max_width() const {
        size_t max_width = 0;
        for (int i = 0, h = height(root_) + 1; i < h; ++i) {
            size_t w = width(root_, i);
            if (w > max_width) {
                max_width = w;
            }
        }
        return max_width;
    }

    /**
     * Returns the successful search cost, i.e. the average number of nodes
     * visited to find a key that is present.
     */
    double successful_search_cost() const {
        return size_ == 0 ? 0 : 1 + (double)sum_levels() / size_;
    }

    /**
     * Returns the unsuccessful search cost, i.e. the average number of nodes
     * visited to find a key that is not present.
     */
    double unsuccessful_search_cost() const {
        return (double)sum_null_levels() / null_count();
    }

    /**
     * Searches for item. If found, returns an iterator pointing
     * at it in the tree; otherwise, returns end().
     */
    iterator find(const K &key) {
        Node<K, V> *x = root_;
        while (x != nullptr) {
            const K& current_key = x->key();
            if (key == current_key) {
                break; // Found!
            } else if (key < current_key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return iterator(x, this);
    }

    /**
     * Returns an iterator pointing to the first item in order.
     */
    iterator begin() {
        Node<K, V> *curr = root_;

        // if the tree is not empty, the first node
        // in order is the farthest node left from root
        if (curr != nullptr) {
            while (curr->left != nullptr) {
                curr = curr->left;
            }
        }

        // build return value using private constructor
        return iterator(curr, this);
    }

    /**
     * Returns an iterator pointing just past the end of the tree data.
     */
    iterator end() {
        return iterator(nullptr, this);
    }

private:
    Node<K, V> *root_;
    size_t size_;
    friend class RedBlackTreeIterator<K, V>;

    /**
     * Deletes all nodes from the red-black tree.
     */
    void delete_tree(Node<K, V> *n) {
        // TODO

    	//keep deleting every node left and right side of the subtrees
    	//and delete the node at the end
    	if (n != nullptr){
    		delete_tree(n->left);
    		delete_tree(n->right);
    		delete n;
    	}

    }

    /**
     * Fixup method described on p. 316 of CLRS.
     */
    void insert_fixup(Node<K, V> *z) {
        // TODO

    	Node<K, V> *y;
    	//adds the conditional of the nullptr to make sure there's a node present
    	//the rest follows the pseudocode given
    	while ((z->parent != nullptr) && (z->parent->color == RED)) {
    		if (z->parent == z->parent->parent->left) {
    			y = z->parent->parent->right;
    			//case 1: z's uncle y is red
    			if ((y != nullptr) && (y->color == RED)) {
    				z->parent->color = BLACK;
    				y->color = BLACK;
    				z->parent->parent->color = RED;
    				z = z->parent->parent;
    			}
    			else {
    				//case 2: z's uncle y is black and z is a right child
    				if (z == z->parent->right) {
    					z = z->parent;
    					left_rotate(z);
    				}
    				//case 3: z's uncle y is black and z is the left child
    				z->parent->color = BLACK;
    				z->parent->parent->color = RED;
    				right_rotate(z->parent->parent);
    			}
    		}
    		else {
    			//mirror version of the above case with left and right
    			y = z->parent->parent->left;
    			//case 1
    			if ((y != nullptr ) && (y->color == RED)) {
    				z->parent->color = BLACK;
    				y->color = BLACK;
    				z->parent->parent->color = RED;
    				z = z->parent->parent;
    			}
    			else {
    				//case 2: z's uncle y is black and z is the left child
    				if (z == z->parent->left) {
    					z = z->parent;
    					right_rotate(z);
    				}
    				//case 3: z's uncle y is black and z is the right child
    				z->parent->color = BLACK;
    				z->parent->parent->color = RED;
    				left_rotate(z->parent->parent);
    			}
    		}
    	}
        // Last line below
        root_->color = BLACK;
    }

    /**
     * Left-rotate method described on p. 313 of CLRS.
     */
    void left_rotate(Node<K, V> *x) {
        // TODO
    	Node<K, V> *y;

    	y = x->right;
    	//if not empty; turns y's left subtree into x's right
    	if(!(y == nullptr)){
    		x->right = y->left;
    		//if not empty; the y's left's parent becomes x
    		if(y->left != nullptr){
    			y->left->parent = x;
    		}
    		//the parent of x is also the parent of y
    		y->parent = x->parent;

    		//if x is the root of the tree; y becomes the root
    		if (x->parent == nullptr){
    			root_ = y;
    		}
    		//x's left subtree of it's parent then y becomes the left subtree
    		//else; y becomes the right subtree
    		else if(x == x->parent->left){
    			x->parent->left = y;
    		}

    		else{
    			x->parent->right = y;
    		}
    		//put y on x's left and y is now the parent of x
    		y->left = x;
    		x->parent = y;
    	}



    }

    /**
     * Right-rotate method described on p. 313 of CLRS.
     */
    void right_rotate(Node<K, V> *x) {
        // TODO

    	Node<K, V> *y;

    	y = x->left;
    	//if not empty; turns y's right subtree into x's left
    	if(!(y == nullptr)){
    		x->left = y->right;
    		//if not empty; the y's right's parent becomes x
    		if(y->right != nullptr){
    			y->right->parent = x;
    		}
    		//the parent of x is also the parent of y
    		y->parent = x->parent;

    		//if x is the root of the tree; y becomes the root
    		if(x->parent == nullptr){
    			root_ = y;
    		}
    		//x's right subtree of it's parent then y becomes the right subtree
    		//else; y becomes the left subtree
    		else if(x == x->parent->right){
    			x->parent->right = y;
    		}
    		else{
    			x->parent->left = y;
    		}
    		//put y on x's right and y is now the parent of
    		y->right = x;
    		x->parent = y;
    	}
    }

    /**
     * Returns the height of the red-black tree starting at node.
     * A null node starts at height -1.
     */
    int height(Node<K, V> *node) const {
        // TODO

    	int count;
    	//null node starts at height of -1
    	if (node == nullptr){
    		count = -1;
    	}
    	else{
    		//recursively calls height function on both left and right nodes
    		//if left > right; count the left nodes; else count the right nodes
    		if (height(node->left) > height(node->right)){
    			count = height(node->left) + 1;
    		}
    		else{
    			count = height(node->right)+1;
    		}
    	}

    	return count;
    }

    /**
     * Returns the count of leaves in the red-black tree starting at node.
     * For this method, a leaf is a non-null node that has no children.
     */
    size_t leaf_count(Node<K, V> *node) const {
        // TODO

    	size_t count = 0;
    	//null node -> no children
    	if (node == nullptr){
    		count = 0;
    	}
    	//if the node doesn't have any left or right subtrees; count = 1;
    	else if ((node->left == nullptr) && (node->right == nullptr)){
    		count++;
    	}
    	//else; count the leaf nodes from both left and right subtrees
    	else{
    		count = leaf_count(node->left)+leaf_count(node->right);
    	}

    	return count;
    }

    /**
     * Returns the count of internal nodes in the red-black tree starting at
     * node.
     * An internal node has at least one child.
     */
    size_t internal_node_count(Node<K, V> *node) const {
        // TODO


    	size_t count = 0;
    	//null node -> no children
    	if(node == nullptr){
    		count = 0;
    	}
    	//no left of right children -> count = 0
    	else if ((node->left == nullptr ) && (node->right == nullptr)){
    		count = 0;
    	}
    	//count the nodes from left and right subtrees
    	else{
    		count = internal_node_count(node->left) + internal_node_count(node->right) + 1;
    	}

    	return count;
    }

    /**
     * Helper method to assist in the computation of tree diameter.
     */
    int diameter(Node<K, V> *node) const {
        // TODO
    	int count;
    	//null node -> 0
    	if (node == nullptr){
    		count = 0;
    	}
    	//max between left and right subtree; max between the diameters
    	else{
    		count = std::max(2+height(node->left) + height(node->right), std::max(diameter(node->left), diameter(node->right)));
    	}
    	return count;

    }

    /**
     * Returns the width of the red-black tree at the designated level.
     * Width is defined as the number of nodes residing at a level.
     */
    size_t width(Node<K, V> *node, size_t level) const {
        // TODO

    	size_t count = 0;
    	//no nodes -> count = 0
    	if(node == nullptr){
    		count = 0;
    	}
    	//if there's only one level -> 1
    	else if (level == 0){
    		count = 1;
    	}
    	//for every node in the left and right subtree of increase the width
    	else{
    		count = width(node->left, level - 1) + width(node->right, level - 1);
    	}
    	return count;

    }

    size_t null_count() const {
        return null_count(root_);
    }

    /**
     * Returns the count of null nodes in the red-black tree starting at node.
     */
    size_t null_count(Node<K, V> *node) const {
        // TODO
    	size_t count = 0;
    	//if the node is null; count = 1
    	if (node == nullptr){
    		count = 1;
    	}
    	//else; recursively call null_count on left and right subtrees and increase count
    	else{
    		count =  null_count(node->left) + null_count(node->right);
    	}
    	return count;
    }

    size_t sum_levels() const {
        return sum_levels(root_, 0);
    }

    /**
     * Returns the sum of the levels of each non-null node in the red-black
     * tree starting at node.
     * For example, the tree
     *   5 <- level 0
     *  / \
     * 2   8 <- level 1
     *      \
     *       10 <- level 2
     * has sum 0 + 2(1) + 2 = 4.
     */
    size_t sum_levels(Node<K, V> *node, size_t level) const {
        // TODO

    	size_t count = 0;
    	//if the node is null; count = 0
    	if (node == nullptr){
    		count = 0;
    	}
    	//counts the level by increasing 1 as it recursively goes through left and right subtrees
    	else{
    		count = level + sum_levels(node->left, level + 1) + sum_levels(node->right, level + 1);
    	}
    	return count;
    }

    size_t sum_null_levels() const {
        return sum_null_levels(root_, 0);
    }

    /**
     * Returns the sum of the levels of each null node in the red-black tree
     * starting at node.
     * For example, the tree
     *    5 <- level 0
     *   / \
     *  2   8 <- level 1
     * / \ / \
     * * * * 10 <- level 2
     *       / \
     *       * * <- level 3
     * has sum 3(2) + 2(3) = 12.
     */
    size_t sum_null_levels(Node<K, V> *node, size_t level) const {
        // TODO
    	//if node is null; return the level
    	if (node == nullptr){
    		return level;
    	}
    	//counts the level by increasing 1 as it recursively goes through left and right subtrees
    	else{
    		return sum_null_levels(node->left, level + 1) + sum_null_levels(node->right, level + 1);
    	}
    }
};

#endif /* RBTREE_H_ */
