#ifndef NODE_H
#define NODE_H

#include "Product.h"

class Node {
private:
    Product data;
    Node* next_node;

public:
    Node() {
        data = Product();
        next_node = nullptr;
    }
    
    Node(Product val, Node* next = nullptr) {
        data = val;
        next_node = next;
    }

    Product retrieve() const { return data; }
    Node* next() const { return next_node; }

    void set_data(Product val) { data = val; }
    void set_next(Node* next) { next_node = next; }

    friend class LinkedList;
    friend class Stack;
    friend class Queue;
};

#endif
