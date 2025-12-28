/**
 * Smart Grocery Cart - Data Structures Project
 * Air University, 3rd Semester
 * 
 * Node.h - Generic Node class for Linked Structures
 */

#ifndef NODE_H
#define NODE_H

#include "Product.h"

/**
 * Node Class (Self-Referential Structure)
 * Building block for LinkedList, Stack, and Queue.
 * Contains DATA (Product) and LINK (pointer to next node).
 */
class Node {
private:
    Product data;
    Node* next_node;

public:
    // Constructors
    Node() : data(Product()), next_node(nullptr) {}
    Node(Product val, Node* next = nullptr) : data(val), next_node(next) {}

    // Accessors
    Product retrieve() const { return data; }
    Node* next() const { return next_node; }

    // Mutators
    void set_data(Product val) { data = val; }
    void set_next(Node* next) { next_node = next; }

    // Friend classes for direct access
    friend class LinkedList;
    friend class Stack;
    friend class Queue;
};

#endif // NODE_H
