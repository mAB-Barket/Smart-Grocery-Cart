/**
 * Smart Grocery Cart - Data Structures Project
 * Air University, 3rd Semester
 * 
 * Stack.h - Stack implementation (LIFO) for Undo operations
 * 
 * Time Complexity (All O(1)):
 *   - push(): Add to top
 *   - pop(): Remove from top
 *   - top(): View top element
 *   - empty(): Check if empty
 */

#ifndef STACK_H
#define STACK_H

#include <iostream>
#include "Node.h"
using namespace std;

/**
 * Stack Class (LIFO - Last In First Out)
 * Used for undo functionality - last action is undone first.
 * Structure: TOP -> [node] -> [node] -> ... -> BOTTOM
 */
class Stack {
private:
    Node* stack_top;
    int stack_size;

public:
    Stack() : stack_top(nullptr), stack_size(0) {}
    ~Stack() { clear(); }

    // Accessors
    bool empty() const { return stack_top == nullptr; }
    int size() const { return stack_size; }
    Node* top_node() const { return stack_top; }

    // View top element without removing - O(1)
    Product top() const {
        if (empty()) return Product();
        return stack_top->retrieve();
    }

    // Push - Add to top - O(1)
    void push(Product val) {
        Node* new_node = new Node(val, stack_top);
        stack_top = new_node;
        stack_size++;
    }

    // Pop - Remove and return top element - O(1)
    Product pop() {
        if (empty()) return Product();
        
        Node* temp = stack_top;
        Product popped_item = temp->retrieve();
        stack_top = stack_top->next();
        delete temp;
        stack_size--;
        return popped_item;
    }

    // Clear all elements - O(n)
    void clear() {
        while (!empty()) {
            pop();
        }
    }

    // Display
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

#endif // STACK_H
