#ifndef STACK_H
#define STACK_H

#include <iostream>
#include "Node.h"
using namespace std;

class Stack {
private:
    Node* stack_top;
    int stack_size;

public:
    Stack() {
        stack_top = nullptr;
        stack_size = 0;
    }
    ~Stack() { clear(); }

    bool empty() const { return stack_top == nullptr; }
    int size() const { return stack_size; }
    Node* top_node() const { return stack_top; }

    Product top() const {
        if (empty()) return Product();
        return stack_top->retrieve();
    }

    void push(Product val) {
        Node* new_node = new Node(val, stack_top);
        stack_top = new_node;
        stack_size++;
    }

    Product pop() {
        if (empty()) return Product();
        
        Node* temp = stack_top;
        Product popped_item = temp->retrieve();
        stack_top = stack_top->next();
        delete temp;
        stack_size--;
        return popped_item;
    }

    void clear() {
        while (!empty()) {
            pop();
        }
    }

    void traverse() const {
        cout << "\n=== UNDO STACK (LIFO) ===" << endl;
        if (empty()) {
            cout << "Stack is empty" << endl;
            return;
        }
        cout << "TOP" << endl;
        for (Node* ptr = stack_top; ptr != nullptr; ptr = ptr->next()) {
            cout << " | " << ptr->retrieve().getName() << endl;
        }
        cout << "BOTTOM" << endl;
    }

    void display_visual() const {
        cout << "TOP -> ";
        if (empty()) {
            cout << "NULL" << endl;
            return;
        }
        for (Node* ptr = stack_top; ptr != nullptr; ptr = ptr->next()) {
            cout << "[" << ptr->retrieve().getName() << "]";
            if (ptr->next() != nullptr) cout << " -> ";
        }
        cout << " -> BOTTOM" << endl;
    }
};

#endif
